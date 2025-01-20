from collections import defaultdict
from typing import Any, Dict, List, Tuple
import sys
import os

import pickle

from dotenv import load_dotenv

import numpy as np

from numpy.typing import NDArray
from sklearn.feature_extraction.text import TfidfVectorizer

import spacy

from hdbscan import HDBSCAN

from langchain_core.documents import Document
from langchain_community.document_loaders import PyMuPDFLoader
from langchain_chroma import Chroma
from langchain_openai import OpenAIEmbeddings


load_dotenv("/home/inri/Projects/Programming/AI/bookycpp/src/python/knowledgebase/.env")
embedding_function = OpenAIEmbeddings()
#embedding_function = HuggingFaceEmbeddings(
#                                model_name="Alibaba-NLP/gte-large-en-v1.5",
#                                model_kwargs={"trust_remote_code": True},
#                                encode_kwargs={"normalize_embeddings": True,
#                                               "batch_size": 16})
#embedding_function.show_progress = True

db_path =  "/home/inri/Projects/Programming/AI/bookycpp/testdb/f86d615c7666658d92037bbdb0cc3054c6dd27412fbed4c2ea352a4e0f429d52/"

def generate_and_store_chroma_db(documents: List, path_to_store: str) -> None:
    documents = split_documents(documents)
    Chroma.from_documents(documents, embedding_function,
                          persist_directory=path_to_store)
    print(f"[+] Chroma vector db persisted at {path_to_store}")


def query_db(query: str, path: str) -> List[Tuple[Document, float]]:
    db = Chroma(persist_directory=path, embedding_function=embedding_function)
    return db.similarity_search_with_relevance_scores(query, 3)

    
def split_documents(documents:List[Document]) -> List[Document]:
    split_documents: List[Document] = []

    nlp = spacy.blank("en")
    nlp.add_pipe("sentencizer")
    
    for doc in documents:
        spacy_doc = nlp(doc.page_content)

        for sent in spacy_doc.sents:
            split_doc = Document(sent.text.strip(), metadata=doc.metadata)
            split_documents.append(split_doc)
    
    print(f"In total {len(documents)} pages have been processed and split into {len(split_documents)} sentences")
    return split_documents
    
def get_embeddings(path_to_chroma_db: str) -> Dict[str, Any]:
    chroma = Chroma(persist_directory=path_to_chroma_db)
    return chroma.get(include=["embeddings", "documents", "metadatas"])

def cluster_embeddings(embeddings: np.ndarray, min_cluster_size=3, min_samples=3, 
                       neighbouring_clusters_epsilon=0.0) -> np.ndarray:
    clusterer = HDBSCAN(min_cluster_size=min_cluster_size, min_samples=min_samples,
                        cluster_selection_epsilon=neighbouring_clusters_epsilon,
                        metric="euclidean")
    return clusterer.fit_predict(embeddings)
    
def group_sentences_by_cluster(sentences: List[str], clustered_sentences: np.ndarray) \
-> Dict[int, List[str]]:
    if len(sentences) != len(clustered_sentences):
        raise RuntimeError("Number of sentences does not match the number of clustered sentences")

    sentences_to_clusters: defaultdict = defaultdict(list)

    for i, cluster in enumerate(clustered_sentences):
        if cluster == -1:
            continue
        sentences_to_clusters[cluster].append(sentences[i])

    return sentences_to_clusters

def extract_keywords_for_clusters(clusters_to_sentences: Dict[int, List[str]], num_of_keywords: int=5)\
-> Dict[int, NDArray]:
    cluster_to_keywords: Dict[int, NDArray] = {}
    
    for cluster, sentences in clusters_to_sentences.items():
        try:
            vectorizer = TfidfVectorizer(max_features=100, stop_words="english")
            tfidf_matrix = vectorizer.fit_transform(sentences)
            feauture_names = vectorizer.get_feature_names_out()
            tfidf_scores_mean = np.asarray(tfidf_matrix.mean(axis=0)).flatten()
            highest_score_indices = tfidf_scores_mean.argsort()[-num_of_keywords:][::-1]
            highest_score_keywords = [feauture_names[i] for i in highest_score_indices]

            cluster_to_keywords[cluster] = highest_score_keywords
        except Exception:
            continue

    return cluster_to_keywords


def generate_main(path_to_doc: str):
    docs: List[Document] = PyMuPDFLoader(path_to_doc).load()
    generate_and_store_chroma_db(docs, db_path)


def query_main(query:str):
    for i, res in enumerate(query_db(query, db_path)):
        print(f"Result {i}:\ntext:{res[0].page_content}\nrelevance: {res[1]}\n"
              f"page: {res[0].metadata}\n\n")


def cluster_main():
    embeddings_with_documents: Dict[str, Any] = get_embeddings(db_path)

    path_to_clusterized_embeddings = db_path + "clusterized_embeddings.pkl"
    if os.path.exists(path_to_clusterized_embeddings):
        print("Found existings clusterized embeddings")
        with open(path_to_clusterized_embeddings, "rb") as f:
            clusterized_embeddings = pickle.load(f)
    else:
        print("Did not found existings clusterized embeddings, creating new. It will take a while...")
        clusterized_embeddings: np.ndarray = cluster_embeddings(embeddings_with_documents["embeddings"],
                                                                min_samples=5, min_cluster_size=6,
                                                                neighbouring_clusters_epsilon=0)
        with open(path_to_clusterized_embeddings, "wb") as f:
            pickle.dump(clusterized_embeddings, f)

    sentences: List[str] = [doc.replace("\n", " ") for doc in embeddings_with_documents["documents"]]
    clusters_to_sentences: Dict[int, List[str]] = group_sentences_by_cluster(
                                                                sentences, clusterized_embeddings)

    clusters_to_keywords: Dict[int, np.ndarray] = extract_keywords_for_clusters(clusters_to_sentences)

    for clust, kw in clusters_to_keywords.items():
        print(f"cluster {clust} : {kw}")


if __name__ == "__main__":
    if sys.argv[1] == "q":
        if len(sys.argv) != 3:
            raise RuntimeError("Need to provide the query string as the second argument")
        query_main(sys.argv[2])
    elif sys.argv[1] == "g":
        if len(sys.argv) != 3:
            raise RuntimeError("Need to provide filepath to document")
        generate_main(sys.argv[2])
    elif sys.argv[1] == "c":
        cluster_main()
    else:
        print("Need to provide mode as the first argument, either g to generate a db or q to query")



