# MMUL — Benchmark de multiplication de matrices

Ce projet permet de mesurer l'impact du cache sur la multiplication de matrices en C, d'automatiser les benchmarks, de générer des graphiques et de compiler un rapport scientifique, le tout de façon reproductible grâce à Docker.

---

## Structure du projet

- `main.c` : code source C des différents algorithmes de multiplication de matrices
- `bin/` : dossier des exécutables compilés
- `data/` : résultats bruts (CSV, images)
- `notebook.ipynb` : notebook Python pour l'automatisation, l'analyse et la génération des graphiques
- `notebook.html`: pour voir l'execution du notebook pendant le docker
- `latex/rapport.pdf` : rapport scientifique générer par LaTeX
- `Dockerfile` : environnement reproductible (Python, Jupyter, LaTeX)
- `Makefile` : automatisation de toutes les étapes (compilation, exécution, analyse, rapport)
- `requirements.txt` : dépendances Python (pandas, plotnine, etc.)

---

## Prérequis

- [Docker](https://www.docker.com/) installé
- `make` installé pour lancer toutes les étapes en une commande

---

## Utilisation rapide

### 1. Tout exécuter automatiquement

À la racine du projet, lance :

```bash
make
```

Cela va :
- Construire l'image Docker avec tous les outils nécessaires
- Compiler le binaire C
- Exécuter le notebook Python (génère les résultats et les graphiques)
- Compiler le rapport LaTeX (PDF dans `latex/`)

### 2. Utilisation manuelle

#### Compilation du binaire

```bash
make docker
make bin/mmul_bench
```

#### Lancer les benchmarks et générer les résultats

```bash
make notebook
```
Cela exécute le notebook dans Docker, génère `data/resultats.csv` et les graphiques.

#### Compiler le rapport

```bash
make latex
```
Cela compile le PDF du rapport dans `latex/`.

#### Nettoyer les fichiers générés

```bash
make clean
```

---

## Description du binaire

```bash
./bin/mmul_bench N <ALGORITHMS>
```

- **N** : taille de la matrice
- **<ALGORITHMS>** : ordre des boucles, parmi `ijk`, `jik`, `kij`, `ikj`, `jki`, `kji`

Exemple :

```bash
./bin/mmul_bench 24 ijk,kji
```

Affiche :

```txt
enabling: 'ijk'
enabling: 'kji'
501601.342243
ijk:354809.000000μs:77673
501601.342243
```

- Les valeurs correspondent à la dernière valeur calculée et au temps total pour chaque algorithme.
- Format : `algo:tempsμs:nb_iter`

---

## Reproductibilité

Tout le projet est conçu pour être exécuté sur n'importe quel système (Linux, Windows, Mac) grâce à Docker.  
Aucune installation manuelle de dépendances Python ou LaTeX n'est nécessaire.

---

## Contact

Nathan Fontaine, Hugo Viala, Dardan Bytyqi