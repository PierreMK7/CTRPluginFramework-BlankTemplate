#include <3ds.h>
#include <CTRPluginFramework.h>

// Définition de votre code d'optimisation (Version Europe)
#define ADRESSE_OPTIMISATION   0x00451C60 
#define VALEUR_OPTIMISATION    0xEA000007

// Cette fonction s'exécute en continu en tâche de fond pendant que vous jouez
void executionPerformance(MenuEntry *entry) {
    if (entry->IsActivated()) {
        // Si l'option est cochée dans votre menu, on force l'injection du code en RAM
        *(volatile u32*)(ADRESSE_OPTIMISATION) = VALEUR_OPTIMISATION;
    }
}

// Configuration de votre menu unique
void InitMenu(PluginMenu *menu) {
    // Création de la ligne cochable dans le menu
    MenuEntry *entry = new MenuEntry("Disable Right Eye Rendering", executionPerformance);
    
    // Description qui s'affiche en bas de l'écran de la console
    entry->SetDescription("Ameliore massivement la fluidite en jeu en coupant le rendu de l'oeil droit.");
    
    // Ajout de l'option au menu principal
    menu->Append(entry);
}

int main(void) {
    // IMPORTANT : On initialise le menu avec les options strictes (1, 0, 0)
    // Cela indique au système CTRPF de désactiver COMPLÈTEMENT le menu de triche ("Search Menu")
    // et l'éditeur hexadécimal. L'utilisateur ne verra QUE votre option.
    PluginMenu *menu = new PluginMenu("MK7 Optimization Plugin", 1, 0, 0);
    
    // On injecte notre ligne cochable
    InitMenu(menu);
    
    // On lance l'exécution du menu
    menu->Run();
    
    return 0;
}
