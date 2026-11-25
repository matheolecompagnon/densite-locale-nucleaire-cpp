# Installation des bindings python

## 1. Créer un venv et se placer dedans

```bash
cd bindings # si ce n'est pas déjà le répertoire courant
python3 -m venv .venv
source .venv/bin/activate
```

## 2. Build et installer le module dans le venv

```bash
make
```


## 3. Tester

```bash
python3 ../tests/python/test_bindings.py
```

## 4. Désinstaller

```bash
make uninstall
```

## 5. Clean

Supprime les fichiers intermédiaires de build.

```bash
make clean
```

