# ctestinglib

**A test runner for C that keeps going when your code crashes, hangs, leaks memory, prints, or runs out of memory.**

**Un lanceur de tests pour le C qui continue même quand votre code plante, boucle, fuit en mémoire, affiche ou manque de mémoire.**

[🇫🇷 Français](#francais) · [🇬🇧 English](#english)

---

<a id="francais"></a>



## 🇫🇷 Français

### Le but du projet

`ctestinglib` est une petite bibliothèque de tests et un lanceur de tests pour le C, sans aucune dépendance externe. Elle permet de décrire le comportement attendu d'un programme sous forme d'un arbre de tests nommés, de lancer cet arbre, d'isoler les échecs et d'obtenir un rapport clair — y compris dans les cas pénibles qui rendent le test du C compliqué : le code écrit sur `stdout`, lit sur `stdin`, provoque un segfault, boucle à l'infini ou manque de mémoire.

L'objectif est simple : pouvoir modifier du code C sans crainte. La bibliothèque est le filet de sécurité ; le rapport indique précisément ce qui a cassé.

### En 30 secondes (pour les non-techniciens)

Voyez `ctestinglib` comme une **checklist automatique pour un programme C**. Chaque test est une question à laquelle on répond par oui ou non : *cette fonction renvoie-t-elle la bonne valeur ? ce programme affiche-t-il le bon texte ? échoue-t-il proprement quand la mémoire manque ?*

Une checklist classique devient peu fiable dès qu'une question fait planter le programme. `ctestinglib` place chaque question dans **sa propre pièce isolée** (un processus séparé), peut donc répondre à toutes les autres, puis vous remet un bulletin de notes : réussi, échoué, planté, expiré — avec la raison.

Les trois programmes exécutables du dossier [`examples/`](examples/) servent à montrer tout cela sans lire une ligne de C : une calculatrice, un lanceur de dés et un jeu fizz-buzz, chacun avec sa propre checklist automatique.

### Le projet en un coup d'œil

| | |
|---|---|
| **Langage** | C99, API POSIX |
| **Dépendances** | Bibliothèque standard C uniquement — aucun paquet tiers |
| **Taille** | Environ 3 600 lignes dans la bibliothèque elle-même : 37 fichiers `.c` et 6 en-têtes |
| **Auto-testé** | 36 tests, exécutés par la bibliothèque elle-même |
| **Exemples** | 3 programmes complets et 25 tests au total |
| **Rapports** | Texte coloré, JSON compact, HTML ou silencieux |
| **Documentation** | Commentaires Doxygen, ce README et trois exemples complets |
| **Plateformes** | Linux avec glibc (plateforme testée) |

### Ce que la bibliothèque sait faire

| Capacité | Ce qu'elle apporte |
|---|---|
| **Arbre de tests** | Regrouper les tests dans des modules et sous-modules nommés ; le rapport affiche chaque groupe et l'ensemble de l'arbre. |
| **Assertions simples** | `ct_assert_true`, `ct_assert_false`, `ct_assert_str_eq` et `ct_assert_str_neq`, avec un message explicatif au format `printf` affiché uniquement en cas d'échec. Un test est une simple fonction qui renvoie `0` (réussi) ou `1` (échoué). |
| **Isolation contre les plantages** | Chaque test s'exécute dans un processus fils séparé. Un segfault est signalé (`KO 139`) et le reste de la suite continue. |
| **Délais maximaux** | Limite par test ou par module ; un test bloqué est tué et signalé comme `KO TIMEOUT`. |
| **Capture de sortie** | Rediriger `stdout` et/ou `stderr` dans un tampon pour le comparer ou l'inspecter, pendant que le code testé continue d'utiliser `printf` normalement. |
| **Capture d'entrée** | Fournir `stdin` à partir d'une chaîne pour chaque cas de test, puis restaurer le vrai `stdin`. |
| **Fixtures** | Crochets `setup` / `teardown` par module, avec la possibilité de couvrir tout le sous-arbre. |
| **Tests paramétrés** | Exécuter la même fonction de test sur un tableau de cas. |
| **Comptage des allocations** | Compter les allocations d'un test avec `ct_alloc_getcount()`. |
| **Injection de panne** | Laisser réussir les `n` allocations suivantes, puis forcer la suivante à échouer avec `ct_failloc_set(n)`, afin de vérifier le chemin d'erreur. |
| **Balayage exhaustif des pannes** | `ct_auto_call(f)` relance `f` une fois par allocation, en faisant échouer une allocation différente à chaque fois — un moyen léger de tester tous les chemins « mémoire insuffisante ». |
| **Nettoyage automatique** | Laisser facultativement le lanceur posséder les allocations suivies : un `free()` en cours de test devient inoffensif, car c'est la passe de nettoyage qui libère réellement. |
| **Rapports** | Uniquement les échecs ou tous les tests, en nombres ou en pourcentages, noms courts ou chemins complets — en texte coloré, JSON compact, HTML ou mode silencieux. |
| **Sélection de modules** | Lancer tout l'arbre ou choisir des modules par chemin (`calculator operators`, `--run-module=parent:child`). |

Une suite volontairement cassée produit quand même un rapport complet au lieu de s'arrêter :

```text
demo	crash demo
	fragile	fragile tests
		too slow  KO	TIMEOUT
	fragile	0 / 1
	segfault  KO	139
demo	0 / 2	[fragile: 0 / 1]
```

### Les exemples (où regarder)

Tout ce qui s'exécute se trouve dans [`examples/`](examples/). Chaque dossier contient un vrai petit programme **et** sa suite de tests complète, compilée contre la bibliothèque comme le ferait un utilisateur externe.

| Exemple | Le programme | Ce que ses tests démontrent |
|---|---|---|
| [`examples/fizzbuzz/`](examples/fizzbuzz/) | La variante classique fizz/buzz, qui lit un nombre sur `stdin`. | Alimenter `stdin` cas par cas, capturer les invites, résultats et erreurs, et vérifier le nombre exact d'allocations. |
| [`examples/calculator/`](examples/calculator/) | Une petite calculatrice interactive (REPL) avec opérateurs, historique et messages d'erreur. | Tests paramétrés, fixtures propriétaires de fichiers temporaires, `stdout` et `stderr` capturés en même temps, comptage d'allocations et échecs forcés. |
| [`examples/dice/`](examples/dice/) | Un lanceur de dés en ligne de commande (`d20 3 --stats`). | Tests d'analyse des arguments, lancers aléatoires avec graine fixe pour un résultat reproductible, analyse des statistiques affichées et tests d'échec d'allocation. |

Compilation et exécution depuis la racine du dépôt :

```sh
make                 # compile la bibliothèque statique
make examples        # compile les trois programmes de démonstration
make test_examples   # compile leurs suites de tests

./examples/fizzbuzz/test_fizzbuzz
./examples/calculator/test_calculator
./examples/dice/test_dice
```

Un module peut être lancé seul, ce qui est pratique pendant le débogage :

```sh
./examples/calculator/test_calculator operators
./examples/calculator/test_calculator --help
```

Sortie réelle de l'exemple fizzbuzz (couleurs retirées) :

```text
fizzbuzz	tests for the fizzbuzz example
	memory	allocation failure showcase
		ct_alloc showcase  OK
	memory	1 / 1
	response	fizz/buzz/bazz responses
		response values  OK
	response	1 / 1
	run	captured stdin and runs
		capture prompts and responses  OK
	run	1 / 1
fizzbuzz	3 / 3	[memory: 1 / 1][response: 1 / 1][run: 1 / 1]
```

### Démarrage rapide

Créez un fichier `my_tests.c` :

```c
#include "ct_assert.h"
#include "ct_modules.h"

static int test_addition(void)
{
	return (ct_assert_true(1 + 1 == 2, "1 + 1 should be 2\n"));
}

int main(int argc, char **argv)
{
	t_module *root;
	int		  ret;

	ct_module_init(&root, "math", "basic arithmetic");
	ct_module_taddf(root, test_addition, "one plus one");
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}
```

Compilez-le contre la bibliothèque puis lancez-le :

```sh
make
clang -std=c99 -Iinclude my_tests.c -L. -lsimple_tests -o my_tests
./my_tests
```

```text
math	basic arithmetic
	one plus one  OK
math	1 / 1
```

Les en-têtes publics sont courts et documentés : [`ct_modules.h`](include/ct_modules.h), [`ct_assert.h`](include/ct_assert.h), [`ct_capture.h`](include/ct_capture.h) et [`ct_memory.h`](include/ct_memory.h).

### La bibliothèque se teste elle-même

Le projet utilise son propre lanceur pour ses propres 36 tests — exactement l'outil que vous venez de voir :

```sh
make tests              # compile le binaire d'auto-test
./simple_tests_tester   # 36 / 36

make coverage           # rapport de couverture HTML dans coverage/
make doxygen            # documentation API dans doxygen/ (nécessite doxygen)
```

### Comment ça marche (pour les curieux)

- Les tests forment un **arbre** : les modules contiennent des tests et des sous-modules, et le chemin d'un module est simplement la chaîne de noms depuis la racine.
- Chaque test s'exécute dans un **processus fils créé par `fork`**. Le fils se termine avec le résultat du test ; le père le récupère avec `waitpid`, le tue éventuellement après un délai maximal, et enregistre un plantage comme un échec normal.
- Le rapport est produit par des **moteurs de rendu séparés** à partir du même arbre de résultats, ce qui garantit la cohérence entre texte, JSON et HTML.
- `ct_capture` redirige un descripteur de fichier dans un tube avec `dup2`, le lit sans bloquer, et restaure le descripteur d'origine à la destruction.
- La couche mémoire encapsule `malloc`, `calloc`, `realloc` et `free` pour compter les allocations, en forcer une à échouer, ou différer le nettoyage.

### Ce que ce projet démontre

- Programmation système en C : processus, signaux, tubes, descripteurs de fichiers, délais maximaux et chemins d'erreur.
- Conception d'API avec types opaques, propriété claire et surface publique réduite.
- Rapports extensibles : un seul arbre de résultats, plusieurs formats de sortie.
- Instrumentation mémoire et injection de pannes — tester le code que l'on ne teste normalement jamais.
- Expérience développeur : un Makefile avec cibles de couverture et de documentation, une interface en ligne de commande, trois exemples complets et une bibliothèque qui se teste elle-même.

### Prérequis

- Linux avec glibc. Le lanceur a besoin de `fork`, `waitpid`, `dup2` et de tubes ; la couche mémoire appelle en plus des fonctions internes de glibc, donc macOS et musl ne sont pas encore pris en charge.
- Un compilateur C99. Le `Makefile` principal cherche actuellement `clang` ; la bibliothèque est du C99 standard et se compile aussi avec `gcc`.
- Facultatif : `llvm-profdata` et `llvm-cov` pour `make coverage`, `doxygen` pour `make doxygen`.

### Les valeurs par défaut (et comment les changer)

| Élément | Valeur par défaut | Comment la changer |
|---|---|---|
| Délai maximal d'un test | Désactivé (`0`) | `ct_test_set_timeout()`, `ct_module_set_timeout()` ou compilation avec `-DCT_DEFAULT_TIMEOUT=...` |
| Nettoyage mémoire automatique | Désactivé | `ct_module_set_auto_cleanup(module, true)` |
| Rapport | Texte coloré, échecs uniquement, noms courts, nombres | Combiner les options passées à `ct_module_run_args()` ou `ct_module_display()` |
| Isolation des tests | Toujours active (un `fork` par test) | Pas de désactivation |
| Ligne de commande | Aucun argument signifie « lancer tout l'arbre » | Passer des noms de modules ou `--run-module=PATH` |
| Injection d'échec d'allocation | Désactivée | `ct_failloc_set(n)` |

### Limites connues, ce qui manque et ce que je ferais ensuite

C'est un projet personnel, et il assume ses limites actuelles :

**Lanceur**

- Le code de sortie du processus ne reflète pas encore un échec d'assertion : une exécution avec des tests en échec peut tout de même sortir avec `0`. En attendant une correction, la CI doit inspecter le rapport JSON plutôt que se fier uniquement au code de sortie.
- Les tests s'exécutent séquentiellement. Une grosse suite n'utilise pas encore plusieurs cœurs.
- Seuls des modules entiers peuvent être sélectionnés ; on ne peut pas lancer un test précis par son nom.

**Assertions**

- Seules les comparaisons booléennes et de chaînes sont fournies. Il n'y a pas d'assertion sur les nombres flottants (avec tolérance), les pointeurs ou les tampons binaires.

**Outils mémoire**

- Les encapsulations de l'allocateur reposent sur des détails internes de glibc (`__libc_malloc` / `__libc_free`) et peuvent donc entrer en conflit avec AddressSanitizer, Valgrind, un autre allocateur ou une libc qui n'est pas glibc.
- Dans un même test, le compteur d'allocations est global au processus : des allocations sans rapport, comme la première utilisation de `stdio`, décalent le compte et rendent les budgets exacts fragiles.
- Il n'y a pas de compteur d'allocations encore vivantes : un test ne peut pas vérifier qu'un chemin d'échec a tout libéré. Le nettoyage automatique masque les fuites au lieu de les signaler.
- `strdup()` et `memdup()` ne sont volontairement pas encapsulés : ils contournent le compteur et l'injection d'échec.

**Fixtures et capture**

- Les crochets de fixture ne prennent pas d'argument : une fixture partagée doit donc vivre dans une variable globale au fichier.
- Les crochets s'exécutent dans le processus père alors que les tests tournent dans des processus fils : les changements d'état faits dans un test sont invisibles pour le `teardown`.
- Il n'y a pas d'aide pour les `FILE *`. `stdin` ne peut être alimenté qu'au niveau du descripteur, et une entrée encore tamponnée peut passer d'un cas à l'autre si elle n'est pas entièrement consommée.
- Les messages d'assertion sont écrits sur `stdout` : ils sont donc avalés pendant qu'une capture de `stdout` est active. Il faut comparer puis détruire la capture avant de faire l'assertion.

**Outillage et diffusion**

- Pas encore de sortie XML/JUnit (un format XML est en pause dans la feuille de route), ce qui limite l'intégration aux tableaux de bord de CI.
- Pas de cible d'installation, de paquet CMake, de fichier `pkg-config`, de version publiée, de workflow CI ni de fichier de licence.
- La sortie HTML est sémantique et autonome, mais livrée sans feuille de style.

**Plateforme**

- Linux/glibc en pratique. Le lanceur n'a besoin que de POSIX, mais les encapsulations de l'allocateur dépendent de fonctions internes de glibc : macOS, musl et Windows ne sont pas encore pris en charge.

**Les prochaines étapes que je prioriserais**

1. Faire échouer le processus quand un test échoue, pour que `make` et la CI s'arrêtent sur une suite rouge.
2. Ajouter une sortie JUnit XML, le langage commun des tableaux de bord de CI.
3. Exécuter les tests en parallèle et ajouter une option `-j`.
4. Rendre l'instrumentation mémoire optionnelle à la compilation pour pouvoir utiliser les sanitizers en parallèle.
5. Ajouter une licence, une cible d'installation, `pkg-config` et un workflow CI avec la couverture.
6. Enrichir les assertions (entiers, flottants, tampons), autoriser des arguments de fixture et sélectionner un test précis par son nom.

### Licence

Aucune licence n'a encore été choisie. C'est l'une des pièces manquantes avant que le projet puisse être réutilisé par d'autres ; une licence permissive comme MIT serait le choix naturel.

---

<a id="english"></a>

## 🇬🇧 English

### The point of the project

`ctestinglib` is a small, dependency-free testing library and test runner for C. It lets a C programmer describe how a program is expected to behave as a tree of named tests, run that tree, isolate the failures, and get a clear report — including for the awkward cases that usually make C testing painful: the code prints on `stdout`, reads from `stdin`, segfaults, loops forever, or runs out of memory.

The goal is simple: make it possible to change C code without fear. The library is the safety net; the report tells you exactly what broke.

### In plain words (30-second summary)

Think of `ctestinglib` as an **automatic checklist for a C program**. Each test is one question with a yes/no answer: *does this function return the right value? does this program print the right thing? does it fail cleanly when memory runs out?*

A classic checklist becomes unreliable as soon as one question makes the program crash. `ctestinglib` puts every question in its **own sealed room** (a separate process), so it can answer all of them anyway, and then hands you a report card: passed, failed, crashed, timed out — with the reason.

The three runnable programs in [`examples/`](examples/) exist to show this without reading any C: a calculator, a dice roller and a fizz-buzz game, each with its own automatic checklist.

### The project at a glance

| | |
|---|---|
| **Language** | C99, POSIX APIs |
| **Dependencies** | C standard library only — no third-party package |
| **Size** | About 3,600 lines in the library itself: 37 `.c` files and 6 headers |
| **Self-tested** | 36 tests, run by the library itself |
| **Examples** | 3 complete programs with 25 tests total |
| **Reports** | Colored text, compact JSON, HTML, or silent |
| **Documentation** | Doxygen comments, this README, and three worked examples |
| **Platforms** | Linux with glibc (the tested platform) |

### What the library can do

| Capability | What it gives you |
|---|---|
| **Test tree** | Group tests into named modules and submodules; the report shows every group and the whole tree. |
| **Simple assertions** | `ct_assert_true`, `ct_assert_false`, `ct_assert_str_eq` and `ct_assert_str_neq`, each with a `printf`-style explanation printed only on failure. A test is just a function returning `0` (pass) or `1` (fail). |
| **Crash-proof isolation** | Every test runs in its own forked process. A segfault is reported (`KO 139`) and the rest of the suite still runs. |
| **Timeouts** | Per-test or per-module limits; a hanging test is killed and reported as `KO TIMEOUT`. |
| **Output capture** | Redirect `stdout` and/or `stderr` into a buffer and compare or inspect it, while the code under test keeps using normal `printf`. |
| **Input capture** | Feed `stdin` from a string for each test case, then restore the real `stdin`. |
| **Fixtures** | `setup` / `teardown` hooks per module, optionally wrapping the whole subtree. |
| **Parametrized tests** | Run the same test function against a table of cases. |
| **Allocation counting** | Count how many allocations a test performs with `ct_alloc_getcount()`. |
| **Failure injection** | Let the next `n` allocations succeed, then force the following one to fail with `ct_failloc_set(n)`, so the error path can be checked. |
| **Exhaustive fault sweep** | `ct_auto_call(f)` runs `f` once per allocation, failing a different allocation each time — a lightweight way to test every out-of-memory path. |
| **Automatic cleanup** | Optionally let the runner own tracked allocations: a mid-test `free()` becomes harmless because the cleanup pass performs the real release. |
| **Reports** | Failures only or every test, counts or percentages, short or full names — in colored text, compact JSON, HTML, or silent mode. |
| **Module selection** | Run the whole tree or pick modules by path (`calculator operators`, `--run-module=parent:child`). |

A deliberately broken suite still produces a full report instead of stopping:

```text
demo	crash demo
	fragile	fragile tests
		too slow  KO	TIMEOUT
	fragile	0 / 1
	segfault  KO	139
demo	0 / 2	[fragile: 0 / 1]
```

### The examples (where to look)

Everything runnable lives in [`examples/`](examples/). Each folder is a real little program **and** its complete test suite, built against the library like an external user would.

| Example | The program | What its tests demonstrate |
|---|---|---|
| [`examples/fizzbuzz/`](examples/fizzbuzz/) | The classic fizz/buzz variant, reading a number from `stdin`. | Feeding `stdin` case by case, capturing prompts/results/errors, and asserting the exact number of allocations. |
| [`examples/calculator/`](examples/calculator/) | A small interactive calculator (REPL) with operators, history and error messages. | Parametrized tests, fixtures that own temporary files, `stdout` and `stderr` captured at the same time, allocation counting and forced failures. |
| [`examples/dice/`](examples/dice/) | A command-line dice roller (`d20 3 --stats`). | Argument-parsing tests, seeded random rolls for reproducible output, parsing printed statistics, and allocation-failure tests. |

Build and run them from the repository root:

```sh
make                 # build the static library
make examples        # build the three demo programs
make test_examples   # build their test suites

./examples/fizzbuzz/test_fizzbuzz
./examples/calculator/test_calculator
./examples/dice/test_dice
```

A module can be run on its own, which is handy while debugging:

```sh
./examples/calculator/test_calculator operators
./examples/calculator/test_calculator --help
```

Real output from the fizzbuzz example (colors removed):

```text
fizzbuzz	tests for the fizzbuzz example
	memory	allocation failure showcase
		ct_alloc showcase  OK
	memory	1 / 1
	response	fizz/buzz/bazz responses
		response values  OK
	response	1 / 1
	run	captured stdin and runs
		capture prompts and responses  OK
	run	1 / 1
fizzbuzz	3 / 3	[memory: 1 / 1][response: 1 / 1][run: 1 / 1]
```

### Quick start

Create a file `my_tests.c`:

```c
#include "ct_assert.h"
#include "ct_modules.h"

static int test_addition(void)
{
	return (ct_assert_true(1 + 1 == 2, "1 + 1 should be 2\n"));
}

int main(int argc, char **argv)
{
	t_module *root;
	int		  ret;

	ct_module_init(&root, "math", "basic arithmetic");
	ct_module_taddf(root, test_addition, "one plus one");
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}
```

Compile it against the library and run it:

```sh
make
clang -std=c99 -Iinclude my_tests.c -L. -lsimple_tests -o my_tests
./my_tests
```

```text
math	basic arithmetic
	one plus one  OK
math	1 / 1
```

The public headers are small and documented: [`ct_modules.h`](include/ct_modules.h), [`ct_assert.h`](include/ct_assert.h), [`ct_capture.h`](include/ct_capture.h) and [`ct_memory.h`](include/ct_memory.h).

### The library tests itself

The project uses its own runner for its own 36 tests — the same tool you just saw:

```sh
make tests              # build the self-test binary
./simple_tests_tester   # 36 / 36

make coverage           # HTML coverage report in coverage/
make doxygen            # API documentation in doxygen/ (needs doxygen)
```

### How it works (for the curious)

- Tests form a **tree**: modules contain tests and submodules, and a module path is simply the chain of names from the root.
- Each test runs in a **forked child process**. The child exits with the test result; the parent collects it with `waitpid`, optionally kills it after a timeout, and records crashes as normal failures.
- The report is produced by **separate renderers** over the same result tree, which is why text, JSON and HTML stay consistent.
- `ct_capture` redirects a file descriptor into a pipe with `dup2`, reads it without blocking, and restores the original descriptor on destroy.
- The memory layer wraps `malloc`, `calloc`, `realloc` and `free` to count allocations, force one to fail, or defer cleanup.

### What this project demonstrates

- Systems programming in C: processes, signals, pipes, file descriptors, timeouts and error paths.
- API design with opaque types, clear ownership, and a public surface that stays small.
- Extensible reporting: one result tree, several output formats.
- Memory instrumentation and fault injection — testing the code that is normally never tested.
- Developer experience: a Makefile with coverage and docs targets, a CLI, three complete examples, and a self-tested library.

### Requirements

- Linux with glibc. The runner needs `fork`, `waitpid`, `dup2` and pipes; the memory layer additionally calls glibc internals, so macOS and musl are not supported yet.
- A C99 compiler. The top-level `Makefile` currently looks for `clang`; the library itself is plain C99 and also builds with `gcc`.
- Optional: `llvm-profdata` and `llvm-cov` for `make coverage`, `doxygen` for `make doxygen`.

### Sensible defaults (and how to change them)

| What | Default | How to change it |
|---|---|---|
| Test timeout | Disabled (`0`) | `ct_test_set_timeout()`, `ct_module_set_timeout()`, or compile with `-DCT_DEFAULT_TIMEOUT=...` |
| Automatic memory cleanup | Off | `ct_module_set_auto_cleanup(module, true)` |
| Report | Colored text, failures only, short names, counts | Combine display flags passed to `ct_module_run_args()` or `ct_module_display()` |
| Test isolation | Always on (one fork per test) | No opt-out |
| Command line | No argument means “run the whole tree” | Pass module names or `--run-module=PATH` |
| Allocation failure injection | Off | `ct_failloc_set(n)` |

### Known limitations, what is missing, and what I would do next

This is a personal project, and it is honest about its current edges:

**Runner**

- The process exit status does not yet reflect a failed assertion: a run with failing tests can still exit with `0`. Until this is fixed, CI should inspect the JSON report rather than only the exit code.
- Tests run sequentially. A large suite does not use several cores yet.
- Only whole modules can be selected; individual tests cannot be run by name.

**Assertions**

- Only boolean and string comparisons are provided. There is no numeric tolerance, pointer, float or raw-buffer assertion.

**Memory tools**

- The allocator wrappers rely on glibc internals (`__libc_malloc` / `__libc_free`), so they can conflict with AddressSanitizer, Valgrind, another allocator, or a non-glibc libc.
- Within one test, the allocation counter is process-wide: unrelated allocations such as the first `stdio` use can shift the count, which makes exact budgets fragile.
- There is no live-allocation counter, so a test cannot assert that a failure path released everything. Automatic cleanup hides leaks instead of reporting them.
- `strdup()` and `memdup()` are intentionally not wrapped, so they bypass the counter and failure injection.

**Fixtures and capture**

- Fixture hooks take no argument, so shared fixtures have to live in file-scope globals.
- Hooks run in the parent process while tests run in forked children: state changes made inside a test are invisible to teardown.
- There is no `FILE *` helper. `stdin` can only be fed at the descriptor level, and buffered input can leak between cases if it is not fully consumed.
- Assertion messages are printed on `stdout`, so they are swallowed while a `stdout` capture is active; the capture must be compared and destroyed first.

**Tooling and packaging**

- No XML/JUnit output yet (an XML format is paused in the roadmap), so integration with CI dashboards is limited.
- No install target, CMake package, `pkg-config` file, versioned release, CI workflow or license file yet.
- HTML output is semantic and self-contained but ships without a stylesheet.

**Platform**

- Linux/glibc in practice. The runner only needs POSIX, but the allocator hooks depend on glibc internals, so macOS, musl and Windows are not supported yet.

**Next steps I would prioritise**

1. Make a failed test fail the process, so `make` and CI stop on a red suite.
2. Add JUnit XML output, the common language of CI dashboards.
3. Run tests in parallel and add a `-j` option.
4. Make the memory instrumentation optional at build time so sanitizers can be used alongside the suite.
5. Add a license, an install target, `pkg-config`, and a CI workflow with coverage.
6. Enrich assertions (integers, floats, buffers), allow fixture arguments, and select single tests by name.

### License

No license has been chosen yet. That is one of the missing pieces before the project can be reused by others; a permissive license such as MIT would be the natural choice.

---

[↑ Back to top / Retour en haut](#ctestinglib)
