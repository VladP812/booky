from typing import List, Tuple

from langchain_core.documents import Document
from langchain_chroma import Chroma
from langchain_huggingface import HuggingFaceEmbeddings
from langchain.text_splitter import RecursiveCharacterTextSplitter

import torch

def generate_and_store_chroma_db(documents: List, 
                                 path_to_store: str) -> None:
    embedding_function = HuggingFaceEmbeddings(
                                    model_name="Alibaba-NLP/gte-large-en-v1.5",
                                    model_kwargs={"trust_remote_code": True},
                                    encode_kwargs={"normalize_embeddings": True,
                                                   "batch_size": 16})
    embedding_function.show_progress = True
    documents = split_documents(documents)
    Chroma.from_documents(documents, embedding_function,
                          persist_directory=path_to_store)
    print(f"[+] Chroma vector db persisted at {path_to_store}")

def query_db(query: str, path: str) -> List[Tuple[Document, float]]:
    embedding_function = HuggingFaceEmbeddings(
                                    model_name="Alibaba-NLP/gte-large-en-v1.5",
                                    model_kwargs={"trust_remote_code": True,
                                                  "device": "cuda"},
                                    encode_kwargs={"normalize_embeddings": True,
                                                   "batch_size": 16})
    db = Chroma(persist_directory=path, embedding_function=embedding_function)
    return db.similarity_search_with_relevance_scores(query, 3)

def split_documents(documents:List[Document], chunk_size:int=2000, 
                    chunk_overlap:int=1000,
                    length_function=len, add_start_index:bool=True):
    splitter = RecursiveCharacterTextSplitter(
            chunk_size=chunk_size,chunk_overlap=chunk_overlap,
            length_function=length_function,add_start_index=add_start_index)
    chunks = splitter.split_documents(documents)
    print(f"In total {len(documents)} documents have been processed and splitted " +
          f"into {len(chunks)} chunks.")
    return chunks

if __name__ == "__main__":
    docs: List[Document] = []
    for i in range(101):
        docs.append(Document(f"page {i}"))
    generate_and_store_chroma_db(docs, "/home/inri/Projects/Programming/AI/bookycpp/testdb/")

