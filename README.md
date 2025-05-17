# 🌙 Luna Game Engine

**Luna** is a modern, cross-platform 2D game engine written in **C++**, featuring support for multiple rendering backends including **Vulkan**, **OpenGL**, and **Direct3D11**. Designed for performance and flexibility, Luna comes with a built-in editor that empowers developers to create, visualize, and iterate on game content in real-time.

---

## ✨ Key Features

- 🔷 **Multi-API Rendering**: Abstract rendering system with support for Vulkan, OpenGL, and Direct3D11.
- 🎮 **2D Game Focused**: Optimized specifically for 2D games, with efficient sprite batching, tilemaps, and animation support.
- 🛠️ **Built-in Editor**: Luna includes a powerful and extensible editor for scene management, entity editing, and live previews.
- 🔌 **Modular Architecture**: Clean, modular codebase designed for easy extension and integration.
- 🚀 **High Performance**: Low-level optimizations with modern C++ practices and hardware-accelerated rendering.
- 🧱 **Entity-Component System (ECS)**: Robust ECS design to manage complex scenes with thousands of entities.
- 📁 **Asset Management**: Integrated pipeline for managing textures, sounds, and other game assets.
- 🌐 **Cross-Platform**: Core engine supports Windows and other platforms (Linux/macOS) with minimal modification.

---

## 🚀 Roadmap
 Basic 2D rendering engine

 Entity-Component System

 Scene serialization

 Integrated editor

 Physics engine

 Scripting support (Lua or similar)

 Audio system

 Networking capabilities

 Linux/macOS support

 ---

## 🧩 Dependencies
| Library    | Purpose         |
| ---------- | --------------- |
| GLFW       | Window & input  |
| GLM        | Math            |
| Vulkan SDK | Vulkan backend  |
| ImGui      | Editor UI       |
| stb\_image | Texture loading |
| spdlog     | Logging         |

---


## 🖥️ The Editor

The Luna Editor is a core part of the development workflow, enabling developers to:

- Create and manage game scenes
- Add, remove, and modify components for each entity
- Preview the game in real time within the editor
- Drag-and-drop assets directly into scenes
- Save and load scenes as JSON or binary formats

The editor is built using native C++ and integrates seamlessly with the underlying engine architecture.

---

## 📷 Screenshots

*(Add screenshots or GIFs here to showcase the engine and editor in action)*

---

## 📦 Getting Started

### Prerequisites

- C++17 compatible compiler (Visual Studio 2019+ or GCC/Clang)
- CMake 3.15+
- Vulkan SDK (for Vulkan support)
- Windows SDK (for Direct3D11 support)
- Git (to clone submodules if used)

### Building the Engine

```bash
git clone https://github.com/yourusername/luna.git
cd luna
mkdir build
cd build
cmake ..
cmake --build .
```

---

## 📚 Documentation
Full documentation is in progress and will cover:

Engine architecture

Editor usage

API references

Tutorials on creating your first Luna game

Stay tuned for updates or check the /docs directory if available.

---

## 🧪 Current Status
Luna is currently under active development. While many core systems are functional, features are being added and refined regularly. Contributions, feedback, and feature requests are welcome!

---

## 🤝 Contributing
Want to contribute to Luna? Feel free to fork the repository, submit pull requests, or open issues for bugs and suggestions. Make sure to follow the contribution guidelines in CONTRIBUTING.md.

---

## 📜 License
Luna is open-source and available under the MIT License. See the LICENSE file for more information.

---

## 💬 Contact
For questions, collaborations, or just to say hi:

GitHub Issues

Discussions tab

Email: [lunaengine@gmail.com] 

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-Windows%2C%20Linux-lightgrey)


