# MMUL

MMUL calcule la multiplication de deux matrices aléatoires de taille $N \times N$.

## Utilisation

```bash
mmul_bench N <ALGORITHMS>
```

**N :** est la taille de la matrice

**<ALGORITHMS> :** est le nom des "algortihmes" utilisées, c'est à dire le nom l'ordre de parcours des indices dans la multiplication. Les différents parcours possibles sont : `ijk`, `jik`, `kij`, `ikj`, `jki` et `kji`.

Le résultat de la fonction est, pour chaque "algorithme", la dernière valeur de la matrice suivi du temps de calcul en micro seconds $(\mu s)$. Si aucun algorithme est fourni, le calcul est effectué sur toutes les combinaisons de $i$, $j$, et $k$ possible. Si une liste est fournie, les résultats affichés sont affichés dans l'ordre `ijk`, `jik`, `kij`, `ikj`, `jki` et `kji`.

Par exemple :

```bash
./mmul_bench 24 ijk,kji
```

Renvoie le résultat :

```bash
enabling: 'ijk'
enabling: 'kji'
501601.342243
ijk:354809.000000μs:77673
501601.342243
```

Les valeurs $501601.342243$ correspondent aux valeurs numériques de la dernière valeur de retour de la matrice. Cet affichage oblige juste le compuleur à faire le calcul (sinon il pourrait supprimer l'intégralité du benchmark). Les autres lignes ont le format suivant :

```txt
	algo:tempsμs:nb_iter
```

**algo :** est le nom de la variante utilisée

**temps :** est le temps *total* de calcul utilisé pour `nb_iter` iterations

**nb\_iter :** est le nombre de fois où la multiplication a été effectuée. Dans le cas des matrices de petite taille, le nombre d'itération est augmenté afin d'avoir un temps de calcul plus stable.