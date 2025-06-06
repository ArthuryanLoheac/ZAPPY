# Project Contribution Guidelines

## 📦 1. Git Commit Convention: Gitmoji

We follow the [Gitmoji](https://gitmoji.dev/) convention to make commit messages expressive and consistent.

### 🔧 Format

```
:emoji: Short, meaningful title
```

Example:

```
✨ Add pathfinding algorithm for enemy AI
```

### 🚀 Common Gitmojis

| Emoji                 | Use case                               |
|-----------------------|----------------------------------------|
| :sparkles:            | New feature                            |
| :bug:                 | Bug fix                                |
| :recycle:             | Code refactor (no new features/fixes)  |
| :lipstick:            | UI/style changes                       |
| :rotating_light:      | Fix compilation errors                 |
| :memo:                | Documentation update                   |
| :construction:        | Work in progress                       |
| :construction_worker: | Add or update CI build system.         |
| :white_check_mark:    | Adding tests                           |
| :ambulance:           | Hotfix                                 |
| :package:             | Adding or updating dependencies        |
| :fire:                | Removing code or files                 |
| :tada:                | Initial commit                         |

Full list available at [https://gitmoji.dev](https://gitmoji.dev)

---

## 🐛 2. Issue Guidelines

Each issue must follow the naming, tagging, and classification standards below to keep project tracking clean and scalable.

### 📝 Issue Title Format

```
Short description
```

Example:

```
Implement basic pathfinding for NPCs
Create responsive HUD for mobile
```

---

### 🚦 Status

| Label             | Description                        |
|-------------------|------------------------------------|
| `status: BackLog` | Not planned right now              |
| `status: todo`    | Planned but not started            |
| `status: doing`   | In progress                        |
| `status: inReview`| Waiting for reviews                |
| `status: done`    | Completed and verified             |

---

## 🏷️ 3. Issue Labels

Use labels to classify and manage issues clearly.

### 🔍 Label

| Label          | Meaning                        |
|----------------|--------------------------------|
| `feature`      | Add or Refactor or improvement |
| `bug`          | A bug or unintended behavior   |
| `documentation`| Documentation-related task     |
| `Tests`        | Testing-related task           |
| `wontfix`      | New feature                    |

### 🧩 Priority

| Label               | Description                        |
|---------------------|------------------------------------|
| `priority: high`    | Must be done ASAP                  |
| `priority: medium`  | Important but not urgent           |
| `priority: low`     | Optional or enhancement only       |

### 📏 Size Estimate

| Label           | Estimated effort                 |
|------------------|----------------------------------|
| `size: XS`       | < 1 hour                         |
| `size: S`        | 1–3 hours                        |
| `size: M`        | 3–6 hours                        |
| `size: L`        | 1–2 days                         |
| `size: XL`       | > 2 days                         |

---

## 🗓 4. Milestones

Every issue should be assigned to a **milestone** matching the current module release cycle:

| Milestone        | Description                                 |
|------------------|---------------------------------------------|
| `Core`           | Logic foundation and structure              |
| `Server`         | Backend APIs and infrastructure             |
| `AI`             | Functional AI behaviors                     |
| `Graphics`       | UI rendering and front-end interactions     |

---