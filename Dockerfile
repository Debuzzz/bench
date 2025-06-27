FROM python:3.11-slim

# Installer les dépendances système, dont le support français pour LaTeX
RUN apt-get update && \
    apt-get install -y build-essential texlive-latex-base texlive-lang-french && \
    rm -rf /var/lib/apt/lists/*

# Installer les dépendances Python
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt jupyter

# Copier le notebook et les fichiers nécessaires
COPY . /workspace
WORKDIR /workspace

CMD ["/bin/bash"]