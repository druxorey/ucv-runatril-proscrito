## Development Guide

### Variable Naming
Variables should follow the `lowelCamelCase` style. Here are some examples:

```c++
int playerScore;
string gameTitle;
bool isGameOver;
```

### File Naming
Files should follow the `kebab-case.cpp` style. Here are some examples:

```
player-score.cpp;
game-title.txt;
is-game-over.hpp;
```

### Commits
For commits, we follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) philosophy. Here are some examples:

```
feat(engine): add collision detection
fix(player): correct health decrement bug
chore(tests): add unit tests for new game feature
docs(readme): update setup instructions
```

### Branching
Never push directly to `main`. Always create a new branch to implement features or make changes. Branches should follow the `type/description` style. Here are some examples:

```
feature/add-new-level
fix/correct-score-calculation
refactor/update-variable-names
chore/update-build-script
```
Once you've finished working on your branch, you can merge it into `main`.
