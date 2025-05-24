// SceneManager.cpp
#include "SceneManager.hpp"
#include "Source/Core/Application.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <imgui_stdlib.h> // Para ImGui::InputText com std::string
#include <filesystem>    // Para manipulação de diretórios (C++17)
#ifdef _WIN32
#include <windows.h> // Para a caixa de diálogo de diretório no Windows
#include <shobjidl.h>
#endif

namespace Luna {

    char Luna::SceneManager::newSceneName[128] = { '\0' };
    std::string Luna::SceneManager::saveDirectory = "./scenes"; // Diretório padrão para salvar

    SceneManager::SceneManager() : m_renderer(nullptr), m_screenWidth(0.0f), m_screenHeight(0.0f) {
        std::cout << "SceneManager construído!" << std::endl;
        std::filesystem::create_directories(saveDirectory); // Cria o diretório se não existir
    }

    SceneManager::~SceneManager() {
        std::cout << "SceneManager destruído!" << std::endl;
    }

    bool SceneManager::Init() {
        std::cout << "SceneManager inicializado." << std::endl;
        return true;
    }

    void SceneManager::Shutdown() {
        m_scenes.clear();
        m_currentScene.reset();
        std::cout << "SceneManager encerrado." << std::endl;
    }

    void SceneManager::addScene(const std::string& name) {
        if (m_scenes.find(name) == m_scenes.end()) {
            m_scenes[name] = std::make_unique<Scene>(m_screenWidth, m_screenHeight, *m_renderer);
            std::cout << "Cena '" << name << "' adicionada." << std::endl;
        }
        else {
            std::cout << "Cena '" << name << "' já existe." << std::endl;
        }
    }

    void SceneManager::loadScene(const std::string& name) {
        if (m_scenes.count(name)) {
            m_currentScene = std::move(m_scenes[name]);
            m_scenes[name] = std::unique_ptr<Scene>(m_currentScene.release());
            std::cout << "Cena '" << name << "' carregada." << std::endl;
        }
        else {
            std::cout << "Cena '" << name << "' não encontrada. Criando uma nova." << std::endl;
            addScene(name);
            loadScene(name);
        }
    }

    void SceneManager::removeScene(const std::string& name) {
        if (m_scenes.erase(name)) {
            std::cout << "Cena '" << name << "' removida." << std::endl;
            if (m_currentScene && m_currentScene->m_renderingSystem.m_world.entities.empty()) {
                m_currentScene.reset();
            }
        }
        else {
            std::cout << "Cena '" << name << "' não encontrada." << std::endl;
        }
    }

    Scene* SceneManager::getCurrentScene() {
        return m_currentScene.get();
    }

    Ecs::Entity SceneManager::createEntity() {
        if (m_currentScene) {
            return m_currentScene->CreateEntity();
        }
        std::cerr << "ERRO: Nenhuma cena atual para criar entidade." << std::endl;
        return {};
    }

    void SceneManager::destroyEntity(Ecs::Entity entity) {
        if (m_currentScene) {
            m_currentScene->DestroyEntity(entity);
        }
        else {
            std::cerr << "ERRO: Nenhuma cena atual para destruir entidade." << std::endl;
        }
    }

    template <typename T, typename... Args>
    T& SceneManager::addComponent(Ecs::Entity entity, Args&&... args) {
        if (m_currentScene) {
            return m_currentScene->AddComponent<T>(entity, std::forward<Args>(args)...);
        }
        throw std::runtime_error("Nenhuma cena atual para adicionar componente.");
    }

    template <typename T>
    T* SceneManager::getComponent(Ecs::Entity entity) const {
        if (m_currentScene) {
            return m_currentScene->GetComponent<T>(entity);
        }
        std::cerr << "ERRO: Nenhuma cena atual para obter componente." << std::endl;
        return nullptr;
    }

    template <typename T>
    bool SceneManager::hasComponent(Ecs::Entity entity) const {
        if (m_currentScene) {
            return m_currentScene->HasComponent<T>(entity);
        }
        std::cerr << "ERRO: Nenhuma cena atual para verificar componente." << std::endl;
        return false;
    }

    void SceneManager::UpdateCurrentScene(float deltaTime) {
        if (m_currentScene) {
            m_currentScene->Update(deltaTime);
        }
    }

    void SceneManager::RenderCurrentScene() {
        if (m_currentScene) {
            m_currentScene->Render();
        }
    }

#ifdef _WIN32
    std::string SceneManager::openFolderDialog() {
        IFileDialog* pfd = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
        if (SUCCEEDED(hr)) {
            DWORD dwOptions;
            if (SUCCEEDED(pfd->GetOptions(&dwOptions))) {
                pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
            }
            if (SUCCEEDED(pfd->Show(nullptr))) {
                IShellItem* psiResult;
                if (SUCCEEDED(pfd->GetResult(&psiResult))) {
                    PWSTR pszPath = nullptr;
                    if (SUCCEEDED(psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszPath))) {
                        std::wstring ws(pszPath);
                        CoTaskMemFree(pszPath);
                        psiResult->Release();
                        pfd->Release();
                        return std::string(ws.begin(), ws.end());
                    }
                    psiResult->Release();
                }
            }
            pfd->Release();
        }
        return "";
    }
#else
    std::string SceneManager::openFolderDialog() {
        // Implementação para outras plataformas (pode usar bibliotecas como tinyfiledialogs)
        std::cout << "Caixa de diálogo de diretório não implementada para esta plataforma." << std::endl;
        return "";
    }
#endif

    void SceneManager::OnImGuiRender() {
        ImGui::Begin("Gerenciador de Cenas");

        static bool showCreateConfirmation = false;
        static float confirmationDisplayTime = 2.0f;
        static float confirmationTimer = 0.0f;

        // --- Adicionar Nova Cena ---
        if (ImGui::CollapsingHeader("Adicionar Cena")) {
            ImGui::InputText("Nome", newSceneName, IM_ARRAYSIZE(newSceneName));
            if (ImGui::Button("Criar Nova Cena")) {
                if (newSceneName[0] != '\0') {
                    addScene(newSceneName);
                    newSceneName[0] = '\0';
                    showCreateConfirmation = true;
                    confirmationTimer = confirmationDisplayTime;
                }
            }
        }

        ImGui::Separator();

        // --- Carregar Cena ---
        if (ImGui::CollapsingHeader("Carregar/Selecionar Cena")) {
            
            if (ImGui::BeginCombo("Cena Atual", m_currentScene ? "Carregada" : "Nenhuma")) {
                for (const auto& [name, scene] : m_scenes) {
                    bool is_selected = (m_currentScene && m_scenes.find(name)->second.get() == m_currentScene.get());
                    if (ImGui::Selectable(name.c_str(), is_selected)) {
                        loadScene(name);
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        ImGui::Separator();
        ImGuiID CENAS_ID = ImGui::GetID("CenasList");
        // --- Gerenciar Cenas Existentes ---
        if (ImGui::CollapsingHeader("Cenas Existentes")) {
            ImGui::Text("Cenas:");
            ImGui::BeginChildFrame(CENAS_ID, ImVec2(-FLT_MIN, 100));
            for (const auto& [name, scene] : m_scenes) {
                ImGui::BulletText("%s", name.c_str());
                ImGui::SameLine();
                if (ImGui::SmallButton(("Carregar##" + name).c_str())) {
                    loadScene(name);
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(("Remover##" + name).c_str())) {
                    removeScene(name);
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(("Salvar##" + name).c_str())) {
                    std::string filepath = (std::filesystem::path(saveDirectory) / ("scene_" + name + ".json")).string();
                    if (m_scenes.count(name)) {
                        m_scenes.at(name)->Serialize(filepath);
                        std::cout << "Cena '" << name << "' salva em: " << filepath << std::endl;
                    }
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(("Carregar do Disco##" + name).c_str())) {
                    std::string filepath = (std::filesystem::path(saveDirectory) / ("scene_" + name + ".json")).string();
                    addScene(name);
                    loadScene(name);
                    if (getCurrentScene() && std::filesystem::exists(filepath)) {
                        if (getCurrentScene()->Deserialize(filepath)) {
                            std::cout << "Cena '" << name << "' carregada do disco." << std::endl;
                        }
                        else {
                            std::cerr << "Erro ao desserializar a cena '" << name << "'." << std::endl;
                        }
                    }
                    else {
                        std::cerr << "Arquivo de cena não encontrado: " << filepath << std::endl;
                    }
                }
            }
            ImGui::EndChildFrame();
        }

        ImGui::Separator();

        // --- Configurações de Salvamento ---
        if (ImGui::CollapsingHeader("Configurações de Salvamento")) {
            ImGui::Text("Diretório de Salvamento:");
            ImGui::InputText("##SaveDirectory", &saveDirectory);
            if (ImGui::Button("Alterar Diretório")) {
                std::string newDir = openFolderDialog();
                if (!newDir.empty()) {
                    saveDirectory = newDir;
                    std::filesystem::create_directories(saveDirectory);
                }
            }
            ImGui::Text("As cenas serão salvas em: %s", saveDirectory.c_str());
        }

        // --- Tela de Confirmação ---
        if (showCreateConfirmation) {
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::OpenPopup("Confirmação de Criação");
            showCreateConfirmation = false; // Abre apenas uma vez por criação
        }

        if (ImGui::BeginPopupModal("Confirmação de Criação", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Nova cena '%s' criada com sucesso!", newSceneName);
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    // Template instantiations (certifique-se de incluir todos os usados)
    template Luna::Ecs::Transform& SceneManager::addComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity, glm::vec3&&, glm::vec3&&, glm::vec3&&);
    template Luna::Ecs::SpriteRenderer& SceneManager::addComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity, const std::string&, const glm::vec4&);

    template Luna::Ecs::Transform* SceneManager::getComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity) const;
    template Luna::Ecs::SpriteRenderer* SceneManager::getComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity) const;

    template bool SceneManager::hasComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity) const;
    template bool SceneManager::hasComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity) const;

} // namespace Luna