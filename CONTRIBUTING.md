# Contribuer au projet INPS23

## Règles

- `main` est protégée : **seul [@matheo.lecompagnon] peut y fusionner**.
- Aucun push direct sur `main`.
- Toute modification passe par une **merge request** (MR).

## Contribuer

**Avec accès au dépôt** : créez une branche (`feat/...`, `fix/...`) et ouvrez une MR vers `main`.

**Sans accès** : forkez le dépôt, travaillez dans votre fork, puis ouvrez une MR vers `main`.

## Avant d'ouvrir une MR

- Une MR = un seul sujet.
- Branche à jour avec `main`.
- Tests et linter OK.
- Description claire (quoi et pourquoi).

## Commits

Format `type: description` (`feat`, `fix`, `docs`, `refactor`, `test`, `chore`).
Exemple : `fix: corrige la pagination`

|Type|	Usage|	Effet sur la version|
|:--|:--|:--|
|feat|	nouvelle fonctionnalité|	MINOR|
|fix|	correction de bug|	PATCH|
|docs|	documentation uniquement|	aucun|
|refactor|	réécriture sans changer le comportement|	aucun|
|test|	ajout ou modification de tests|	aucun|
|chore|	maintenance (dépendances, config, release)|	aucun|
|perf|	amélioration de performance|	PATCH|
|style|	mise en forme (espaces, virgules)|	aucun|


## Relecture

[@matheo.lecompagnon] relit votre MR, peut demander des modifications, puis la fusionne.