#include "Scene.hpp"
#include <iostream> // Para mensagens de log/erro
#include "Source/Core/Log.hpp"

#include "Source/Core/Application.hpp"

#define ICON_FA_CUBE "\xef\x81\xb2"
#define ICON_BOX              "\xef\x91\xa6"

Scene::Scene(const std::string& name)
    : m_name(name) {
    Luna::GLogger.Info("INFO::SCENE: Cena '" + m_name + "' criada.");
    std::cout << "INFO::SCENE: Cena '" << m_name << "' criada." << std::endl;
}

void Scene::AddSceneObject(std::shared_ptr<SceneObject> object) {
    if (!object) {
        Luna::GLogger.Error("ERRO::SCENE::ADD_OBJECT: Tentativa de adicionar um SceneObject nulo à cena '" + m_name + "'.");
        std::cerr << "ERRO::SCENE::ADD_OBJECT: Tentativa de adicionar um SceneObject nulo à cena '" << m_name << "'." << std::endl;
        return;
    }
    // Verifique se o objeto já tem um pai, se sim, não o adicione como raiz.
    // Neste sistema, apenas objetos sem pai são considerados objetos raiz da cena.
    if (!object->GetParent()) {
        m_rootObjects.push_back(object);
        Luna::GLogger.Info("INFO::SCENE: SceneObject '" + object->GetName() + "' adicionado como raiz à cena '" + m_name + "'.");
        std::cout << "INFO::SCENE: SceneObject '" << object->GetName() << "' adicionado como raiz à cena '" << m_name << "'." << std::endl;
    }
    else {
        std::cout << "AVISO::SCENE::ADD_OBJECT: SceneObject '" << object->GetName() << "' já tem um pai. Não adicionado como raiz da cena '" << m_name << "'." << std::endl;
    }
}

void Scene::RemoveSceneObject(const std::string& name) {
    auto it = std::remove_if(m_rootObjects.begin(), m_rootObjects.end(),
        [&](std::shared_ptr<SceneObject>& obj) { return obj->GetName() == name; });

    if (it != m_rootObjects.end()) {
        m_rootObjects.erase(it, m_rootObjects.end());
        std::cout << "INFO::SCENE: SceneObject '" << name << "' removido da cena '" << m_name << "'." << std::endl;
    }
    else {
        std::cout << "AVISO::SCENE::REMOVE_OBJECT: SceneObject '" << name << "' não encontrado na cena '" << m_name << "' para remoção." << std::endl;
    }
}

std::shared_ptr<SceneObject> Scene::GetSceneObjectByName(const std::string& name) {
    for (auto& obj : m_rootObjects) {
        if (obj->GetName() == name) {
            return obj;
        }
        // Se você quiser pesquisar em toda a hierarquia, precisará de uma função recursiva.
        // Por enquanto, esta função só pesquisa nos objetos raiz.
    }
    return nullptr;
}

void Scene::Update(float deltaTime) {
    // Atualiza todos os objetos raiz na cena.
    // SceneObjects filhos serão atualizados recursivamente por seus pais.
    for (auto& object : m_rootObjects) {
        object->Update(deltaTime);
    }
}

void Scene::Render(Renderer2D& renderer) {
    // Renderiza todos os objetos raiz na cena.
    // SceneObjects filhos serão renderizados recursivamente por seus pais/componentes.
    for (auto& object : m_rootObjects) {
        object->Render(renderer);
    }
}

void Scene::OnGui() {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_AllowItemOverlap |
        ImGuiTreeNodeFlags_FramePadding;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

    ImGui::Separator();

    std::string label = "Scene: " + m_name + "##SceneNode_" + m_name;
    if (ImGui::TreeNodeEx(label.c_str(), flags)) {

        ImGui::Spacing();

        if (ImGui::TreeNodeEx("\xef\x80\xbe " " Root Scene Objects ##root", flags)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Scene Objects");

            for (auto& object : m_rootObjects) {
                object->OnGui();
            }

            ImGui::TreePop(); // fecha Root Scene Objects
        }

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::TreePop(); // fecha Scene: <name>
    }

    ImGui::PopStyleVar(2);
}
