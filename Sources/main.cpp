#include <CTRPluginFramework.hpp>

using namespace CTRPluginFramework;

// TitleID de MK7 par région (les 16 derniers chiffres hexa se terminent par 30600/30700/30800)
#define TID_MK7_JP  0x0004000000030600ULL
#define TID_MK7_EU  0x0004000000030700ULL
#define TID_MK7_US  0x0004000000030800ULL

struct CodeDef {
    const char* name;
    const char* description;
    u32 addr_jp;
    u32 addr_eu;
    u32 addr_us;
    u32 value;
};

CodeDef codes[] = {
    { "Optimisation rendu", "Ameliore la fluidite en jeu.",
      0x00451BE4, 0x00451C60, 0x00451BE0, 0xEA000007 },
    // ajoute une ligne par code, avec les 3 adresses régionales + la valeur commune
};

u32 GetRegionAddress(CodeDef *def) {
    u64 tid = Process::GetTitleID();
    if (tid == TID_MK7_JP) return def->addr_jp;
    if (tid == TID_MK7_EU) return def->addr_eu;
    if (tid == TID_MK7_US) return def->addr_us;
    return 0; // région non reconnue, sécurité : n'écrit rien
}

void ApplyCode(MenuEntry *entry) {
    if (entry->IsActivated()) {
        CodeDef *def = (CodeDef *)entry->GetArg();
        u32 addr = GetRegionAddress(def);
        if (addr != 0) {
            *(volatile u32 *)(addr) = def->value;
        }
    }
}

int main(void) {
    FwkSettings::Get().AllowSearchEngine = false;
    FwkSettings::Get().AllowActionReplay = false;

    PluginMenu *menu = new PluginMenu("MK7 CTs+ Plugin", 1, 0, 0);

    for (auto &def : codes) {
        MenuEntry *entry = new MenuEntry(def.name, ApplyCode, def.description);
        entry->SetArg(&def);
        menu->Append(entry);
    }

    menu->Run();
    return 0;
}
