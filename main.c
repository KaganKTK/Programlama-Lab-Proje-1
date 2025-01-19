#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <raylib.h>
#include <curl/curl.h>

#define JSON_LEN 2048
#define LINE_LEN 128
#define OBJ_NAME_LEN 64
#define UNIT_CAP 5
#define HERO_CAP 10
#define CREATURE_CAP 10
#define RESEARCH_CAP 16

#define GRIDSTART_X 355
#define GRIDSTART_Y 20

#define DrawTextA DrawText
#define PlaySoundA PlaySound

struct Research
{
    char name[OBJ_NAME_LEN];
    int level;
    char upgradetype[OBJ_NAME_LEN];
    char targetunit[OBJ_NAME_LEN];
    int upgradevalue;
};

struct Creature
{
    char name[OBJ_NAME_LEN];
    char effecttype[OBJ_NAME_LEN];
    char targetunit[OBJ_NAME_LEN];
    int effectvalue;
};

struct Hero
{
    char name[OBJ_NAME_LEN];
    char bonustype[OBJ_NAME_LEN];
    char targetunit[OBJ_NAME_LEN];
    int bonusvalue;
};

struct Unit
{
    char name[OBJ_NAME_LEN];
    int atk;
    int def;
    int hp;
    int crit;
};

struct Faction
{
    char name[OBJ_NAME_LEN];
    struct Unit unit[UNIT_CAP];
    struct Hero hero[HERO_CAP];
    struct Creature creature[CREATURE_CAP];
};

struct Current_Units
{
    struct Unit unit;
    int sayi;
};

struct Senaryo{

    struct Current_Units CU[UNIT_CAP];
    char Hero_name[OBJ_NAME_LEN];
    char creature_name[OBJ_NAME_LEN];
    char research_name[OBJ_NAME_LEN];
    int res_lvl;
};

void WriteJSON(int x);
size_t GetSizeJSON(void *ptr, size_t size, size_t nmemb, FILE *stream);
void MyDrawGrid(void);
void LoadImages(Texture2D insanasker[UNIT_CAP], Texture2D orkasker[UNIT_CAP], struct Senaryo insanordusu, struct Senaryo orkordusu, Texture2D *humanhero, Texture2D *humanbeast, Texture2D *orchero, Texture2D *orcbeast);
void DrawBirimler(Texture2D insanasker[UNIT_CAP], Texture2D orkasker[UNIT_CAP], struct Senaryo insanordusu, struct Senaryo orkordusu, struct Faction insanimp, struct Faction orklegi);
void DrawKahramanVeYaratiklar(struct Senaryo insanordusu, struct Senaryo orkordusu, Texture2D humanhero, Texture2D humanbeast, Texture2D orchero, Texture2D orcbeast, int winningside);
void PutUnitAndItsNumber(Texture2D texture, Color unittint, short unitnumber, int posX, int posY, Color texttint, struct Unit gazi, struct Unit savaskimligi);
void DrawTextureOnCenter(Texture2D texture, int posX, int posY, Color tint){DrawTexture(texture, posX-(texture.width-40)/2, posY-(texture.height-40)/2, tint); return;}
int FindIdealFontSize(char *texttobeprinted, int limitinpixels){ if(2>strlen(texttobeprinted))return 0; short i; for(i=1; limitinpixels>MeasureText(texttobeprinted, i) ;i++){} return i; }
void ArastirmalariYedim(struct Research *research, struct Faction *insanimp, struct Faction *orklegi);
void KahramanlariYedim(struct Faction *insanimp, struct Faction *orklegi);
void YaratiklariYedim(struct Faction *insanimp, struct Faction *orklegi);
void BirimleriYedim(struct Faction *insanimp, struct Faction *orklegi);
void JsonBitirici(const char *filepath, const short factionno, const short layer3pick, char *assigname1, char *assigname2, const char *expect1, char *assign1, const char *expect2, char *assign2, const char *intexpect1, int *intassign1, const char *intexpect2, int *intassign2, const char *intexpect3, int *intassign3, const char *intexpect4, int *intassign4, const struct Faction insanlartamliste, const struct Faction orklartamliste, const short aciklamacekis1, char *acikassign1, char *acikassign2);
short stroke(char line[LINE_LEN], short startpos, char returnage[OBJ_NAME_LEN]);
void SenaryoyuYedim(const char *filepath, struct Senaryo *ordu, const struct Faction Devlet, const struct Research Arastirma[RESEARCH_CAP], const short whichside);
void Givestat(struct Faction *insanimp, struct Faction *orglegi, struct Senaryo *insanordusu, struct Senaryo *orkordusu);
void BuffsAndNerfs(struct Faction *insanimp, struct Faction *orglegi, struct Senaryo *insanordusu, struct Senaryo *orkordusu,struct Research *Research);
int Simulation(struct Senaryo *insanordusu,struct Senaryo *orkordusu);
int IsBattleOver(struct Senaryo *GoofyAhhStruct);
int CalculateDamage(struct Senaryo *GoofyAhhStruct,int tur);
float CalculateCrit(struct Senaryo *GoofyAhhStruct,int Tur,int number);
int CalculateDefense(struct Senaryo *GoofyAhhStruct);
void CalculateFatigue(struct Senaryo *GoofyAhhStruct,int tur);

int main(){
    //structlar adlandirilir ve temzilenir
    struct Faction insanimp;
    struct Faction orklegi;
    struct Research research[RESEARCH_CAP];
    struct Senaryo insanordusu;
    struct Senaryo orkordusu;
    memset(&insanimp, 0, sizeof(insanimp));
    memset(&orklegi, 0, sizeof(orklegi));
    memset(&research, 0, sizeof(research));
    memset(&insanordusu, 0, sizeof(insanordusu));
    memset(&orkordusu, 0, sizeof(orkordusu));
    // main() fonksiyonundaki uc parantezliler debug araclari, kod calisiyor mu diye bakmana yariyor.
    BirimleriYedim(&insanimp, &orklegi);
    ///for(short i=0; i<UNIT_CAP ; i++) printf("%s\n\t%d\n\t%d\n\t%d\n\t%d\n", insanimp.unit[i].name, insanimp.unit[i].atk, insanimp.unit[i].def, insanimp.unit[i].hp, insanimp.unit[i].crit);
    ///for(short i=0; i<UNIT_CAP ; i++) printf("%s\n\t%d\n\t%d\n\t%d\n\t%d\n", orklegi.unit[i].name, orklegi.unit[i].atk, orklegi.unit[i].def, orklegi.unit[i].hp, orklegi.unit[i].crit);
    YaratiklariYedim(&insanimp, &orklegi);
    ///for(short i=0; i<CREATURE_CAP ;i++) printf("%s\n\t%s\n\t%s\n\t%d\n", insanimp.creature[i].name, insanimp.creature[i].effecttype, insanimp.creature[i].targetunit, insanimp.creature[i].effectvalue);
    ///for(short i=0; i<CREATURE_CAP ;i++) printf("%s\n\t%s\n\t%s\n\t%d\n", orklegi.creature[i].name, orklegi.creature[i].effecttype, orklegi.creature[i].targetunit, orklegi.creature[i].effectvalue);
    KahramanlariYedim(&insanimp, &orklegi);
    ///for(short i=0; i<HERO_CAP ;i++) printf("%s\n\t%s\n\t%s\n\t%d\n", insanimp.hero[i].name, insanimp.hero[i].bonustype, insanimp.hero[i].targetunit, insanimp.hero[i].bonusvalue);
    ///for(short i=0; i<HERO_CAP ;i++) printf("%s\n\t%s\n\t%s\n\t%d\n", orklegi.hero[i].name, orklegi.hero[i].bonustype, orklegi.hero[i].targetunit, orklegi.hero[i].bonusvalue);
    ArastirmalariYedim(research, &insanimp, &orklegi);
    ///for(short i=0; i<RESEARCH_CAP ;i++) printf("%s\n\t%d\n\t%s\n\t%s\n\t%d\n", research[i].name, research[i].level, research[i].upgradetype, research[i].targetunit, research[i].upgradevalue);


    const int ScreenWidth = 1536; const int ScreenHeight = 864;             // ekran ebatlari
    int WhichScreenAmIOn = 0;                       // kac numarali ekranda bulundugumuzun bilgisini saklar
    int SelectedJson = 1;                           // kac numarali jsonun okunacagi bu degiskenden sorulur
    int didweloadimages=0;                          // resimlerin surekli tekrardan yuklenmesini engeller
    char TextDrawer[LINE_LEN] = {0};                // yazi cizici degil, yazi cekmecesi, yazdirilacak seyleri once buna bufferliyorum
    int winningside = 0;                            // kazanan tarafi saklayacak olan o integer
    Texture2D humanhero; Texture2D orchero;
    Texture2D humanbeast; Texture2D orcbeast;
    InitWindow(ScreenWidth, ScreenHeight, "Malazgirt Savasi 1071 (renklendirilmis)");
    Texture2D explosion = LoadTexture("Gorsel/explosion.png");
    InitAudioDevice();
    Sound EXPLOSION = LoadSound("Ses/explosion.wav");
    Vector2 explosiondrawer;
    Rectangle explosionchunk;
    explosionchunk.height=100;
    explosionchunk.width=72;
    explosionchunk.x=0;
    explosionchunk.y=0;
    Rectangle explosionsize;
    explosionsize.height=2640;
    explosionsize.width=1900;
    explosionsize.x=-200;
    explosionsize.y=-700;

    bool didwesimulate = false;
    bool exptrigger = false;
    short explosionframe = 0;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        switch(WhichScreenAmIOn)
        {
            case 0:
                //////////////////// TAKING INPUT AND PREPARATIONS
                SelectedJson += (int)(GetMouseWheelMove());if(SelectedJson<1)SelectedJson=1;if(SelectedJson>10)SelectedJson=10;
                if(IsKeyPressed(KEY_ENTER))
                {
                    char ScenarioFile[LINE_LEN] = {0};
                    sprintf(ScenarioFile, "Files/%d.json", SelectedJson);
                    WriteJSON(SelectedJson);
                    SenaryoyuYedim(ScenarioFile, &insanordusu, insanimp, research, 1);
                    SenaryoyuYedim(ScenarioFile, &orkordusu, orklegi, research, -1);

                    ///printf("insanlar\n\t%s\n\t%s\n\t%s: %d\n", insanordusu.Hero_name, insanordusu.creature_name, insanordusu.research_name, insanordusu.res_lvl);
                    ///for(short i=0; i<UNIT_CAP ;i++) printf("\t%s: %d\n", insanordusu.CU[i].unit.name, insanordusu.CU[i].sayi);
                    ///printf("orklar\n\t%s\n\t%s\n\t%s: %d\n", orkordusu.Hero_name, orkordusu.creature_name, orkordusu.research_name, orkordusu.res_lvl);
                    ///for(short i=0; i<UNIT_CAP ;i++) printf("\t%s: %d\n", orkordusu.CU[i].unit.name, orkordusu.CU[i].sayi);

                    WhichScreenAmIOn=1;

                }
                //////////////////// DRAWING HERE
                BeginDrawing();
                DrawText("Kac numarali jsonu indirmek istediginizi seciniz.", 70, 100, 60, GRAY);
                sprintf(TextDrawer, "%d", SelectedJson);
                DrawText(TextDrawer, ScreenWidth/2-25, ScreenHeight/2-25, 150,GRAY);
                ClearBackground(LIGHTGRAY);
                EndDrawing();
                //////////////////// END
                break;
            case 1:
                printf("."); // this is our coconut.jpg
                //////////////////// LOADING AND PREPARATIONS
                Texture2D insanasker[UNIT_CAP];
                Texture2D orkasker[UNIT_CAP];
                if(didweloadimages==0){LoadImages(insanasker, orkasker, insanordusu, orkordusu, &humanhero, &humanbeast, &orchero, &orcbeast);didweloadimages++;}
                srand(0);

                //////////////////// DRAWING HERE
                BeginDrawing();
                ClearBackground(BLACK);
                DrawKahramanVeYaratiklar(insanordusu, orkordusu, humanhero, humanbeast, orchero, orcbeast, winningside);
                MyDrawGrid();
                DrawBirimler(insanasker, orkasker, insanordusu, orkordusu, insanimp, orklegi);
                if(exptrigger==true)
                {
                    explosionchunk.x = 1+((explosionframe/5)%6)*72;
                    explosionchunk.y = 15+((explosionframe/5)/6)*100;
                    if(explosionframe==80)
                    {
                        exptrigger = false;
                        explosionframe = 0;
                    }
                    explosionframe++;
                    DrawTexturePro(explosion, explosionchunk, explosionsize, explosiondrawer, 0, WHITE);
                    if(explosionframe>39 && didwesimulate==false)
                    {
                        WhichScreenAmIOn=2;
                        didwesimulate = true;
                    }
                }
                EndDrawing();
                //////////////////// END
                if(IsKeyPressed(KEY_ENTER))
                {
                    exptrigger = true;
                    PlaySound(EXPLOSION);
                }
                break;
            case 2:
                //////////////////// ISNT ACTUALLY A SCREEN IT JUST DOES THE SIMULATION AND PASSES IT BACK TO SCREEN 1
                Givestat(&insanimp,&orklegi,&insanordusu,&orkordusu);
                BuffsAndNerfs(&insanimp, &orklegi, &insanordusu, &orkordusu, research);
                winningside = Simulation(&insanordusu,&orkordusu);
                WhichScreenAmIOn=1;
                //////////////////// END
                break;
            }
    }
    CloseAudioDevice();
    CloseWindow();
}

void MyDrawGrid(void)
{
    Color greentones = {0};
    for(int y=0; y<20 ;y++)
    {
        for(int x=0; x<20 ;x++)
        {
            greentones = (Color){rand()%90, 120+rand()%55, rand()%50, 255};
            DrawRectangle(GRIDSTART_X+41*x, GRIDSTART_Y+41*y, 40, 40, greentones);
            DrawRectangleLines((GRIDSTART_X+41*x)-1, (GRIDSTART_Y+41*y)-1, 42, 42, DARKBROWN);
        }
    }
}

void LoadImages(Texture2D insanasker[UNIT_CAP], Texture2D orkasker[UNIT_CAP], struct Senaryo insanordusu, struct Senaryo orkordusu, Texture2D *humanhero, Texture2D *humanbeast, Texture2D *orchero, Texture2D *orcbeast)
{
    char imagesfilepath[LINE_LEN];
    sprintf(imagesfilepath, "Gorsel/%s.png", insanordusu.Hero_name);        *humanhero = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", insanordusu.creature_name);    *humanbeast = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", orkordusu.Hero_name);          *orchero = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", orkordusu.creature_name);      *orcbeast = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", insanordusu.CU[0].unit.name);  insanasker[0] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", insanordusu.CU[1].unit.name);  insanasker[1] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", insanordusu.CU[2].unit.name);  insanasker[2] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", insanordusu.CU[3].unit.name);  insanasker[3] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", orkordusu.CU[0].unit.name);    orkasker[0] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", orkordusu.CU[1].unit.name);    orkasker[1] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", orkordusu.CU[2].unit.name);    orkasker[2] = LoadTexture(imagesfilepath);
    sprintf(imagesfilepath, "Gorsel/%s.png", orkordusu.CU[3].unit.name);    orkasker[3] = LoadTexture(imagesfilepath);
}

void DrawKahramanVeYaratiklar(struct Senaryo insanordusu, struct Senaryo orkordusu, Texture2D humanhero, Texture2D humanbeast, Texture2D orchero, Texture2D orcbeast, int winningside)
{
    DrawText("insanlarin lideri:", 55, 60, 30, WHITE);
    DrawText(insanordusu.Hero_name, 55, 350, FindIdealFontSize(insanordusu.Hero_name, 240), WHITE);
    DrawText("YOK", 145, 205, 30, WHITE);
    DrawTexture(humanhero, 55, 100, WHITE);
    if(winningside==-1) DrawText("x", 55, 0, 440, RED);
    DrawText("insanlarin canavari:", 55, 480, 25, WHITE);
    DrawText(insanordusu.creature_name, 55, 770, FindIdealFontSize(insanordusu.creature_name, 240), WHITE);
    DrawText("YOK", 145, 625, 30, WHITE);
    DrawTexture(humanbeast, 55, 520, WHITE);
    if(winningside==-1) DrawText("x", 55, 420, 440, RED);
    DrawText("orklarin lideri:", 1235, 60, 30, WHITE);
    DrawText(orkordusu.Hero_name, 1235, 350, FindIdealFontSize(orkordusu.Hero_name, 240), WHITE);
    DrawText("YOK", 1325, 205, 30, WHITE);
    DrawTexture(orchero, 1235, 100, WHITE);
    if(winningside==1) DrawText("x", 1235, 0, 440, RED);
    DrawText("orklarin canavari:", 1235, 480, 25, WHITE);
    DrawText(orkordusu.creature_name, 1235, 770, FindIdealFontSize(orkordusu.creature_name, 240), WHITE);
    DrawText("YOK", 1325, 625, 30, WHITE);
    DrawTexture(orcbeast, 1235, 520, WHITE);
    if(winningside==1) DrawText("x", 1235, 420, 440, RED);
}

void DrawBirimler(Texture2D insanasker[UNIT_CAP], Texture2D orkasker[UNIT_CAP], struct Senaryo insanordusu, struct Senaryo orkordusu, struct Faction insanimp, struct Faction orklegi)
{
    int y=0;
    for(short i=0, x=0; i<UNIT_CAP ;i++, x+=2)
    {
        int askersayaci=0;
        for(askersayaci = insanordusu.CU[i].sayi, y=0; askersayaci>100 ;askersayaci-=100, y+=2)
        {
            PutUnitAndItsNumber(insanasker[i], WHITE, 100, GRIDSTART_X+41*x, GRIDSTART_Y+41*y, WHITE, insanordusu.CU[i].unit, insanimp.unit[i]);
        }
        if(insanordusu.CU[i].sayi>0) PutUnitAndItsNumber(insanasker[i], WHITE, askersayaci, GRIDSTART_X+41*x, GRIDSTART_Y+41*y, WHITE, insanordusu.CU[i].unit, insanimp.unit[i]);
    }
    y=0;
    for(short i=0, x=0; i<UNIT_CAP ;i++, x-=2)
    {
        int askersayaci=0;
        for(askersayaci = orkordusu.CU[i].sayi, y=0; askersayaci>100 ;askersayaci-=100, y+=2)
        {
            PutUnitAndItsNumber(orkasker[i], WHITE, 100, GRIDSTART_X+780+41*x, GRIDSTART_Y+41*y, WHITE, orkordusu.CU[i].unit, orklegi.unit[i]);
        }
        if(orkordusu.CU[i].sayi>0) PutUnitAndItsNumber(orkasker[i], WHITE, askersayaci, GRIDSTART_X+780+41*x, GRIDSTART_Y+41*y, WHITE, orkordusu.CU[i].unit, orklegi.unit[i]);
    }
}

void PutUnitAndItsNumber(Texture2D texture, Color unittint, short unitnumber, int posX, int posY, Color texttint, struct Unit gazi, struct Unit savaskimligi)
{
    char *string = malloc(8);
    DrawTextureOnCenter(texture, posX, posY, unittint);
    sprintf(string, "%d", unitnumber);
    DrawText(string, posX+5, posY+45, 20, texttint);
    float kalancan = (float)gazi.hp/(float)savaskimligi.hp;
    DrawRectangle(posX-5, posY+40, 50, 5, BLACK);
    int redval=255, greenval=0;
    if(kalancan==0) kalancan=1;
    int rgbadjuster=kalancan*510;
    if(rgbadjuster>=255)
    {
        greenval+=255;
        rgbadjuster-=255;
        redval-=rgbadjuster;
    }
    else greenval+=rgbadjuster;
    rgbadjuster=0;
    Color hpbar = {redval, greenval, 0, 255};
    DrawRectangle(posX-5, posY+40, 50*kalancan, 5, hpbar);
    DrawRectangleLines(posX-5, posY+40, 50*kalancan, 5, BLACK);
    //sprintf(string, "%d", kalancan);
    //DrawText(string, posX-5, posY+40, 30, RED);
    return;
}

void BirimleriYedim(struct Faction *insanimp, struct Faction *orklegi)
{
    char strslot1[OBJ_NAME_LEN]={0};
    int intslot1=0, intslot2=0, intslot3=0, intslot4=0;
    int i=0;
    for(i=1; i<(UNIT_CAP+1) ;i++)
    {
        JsonBitirici("Files/unit_types.json", 1, i, strslot1, 0, 0, 0, 0, 0, "saldiri", &intslot1, "savunma", &intslot2, "saglik", &intslot3, "kritik_sans", &intslot4, *insanimp, *orklegi, 0, 0, 0);
        strcpy(insanimp->unit[i-1].name, strslot1);
        insanimp->unit[i-1].atk=intslot1;
        insanimp->unit[i-1].def=intslot2;
        insanimp->unit[i-1].hp=intslot3;
        insanimp->unit[i-1].crit=intslot4;
        memset(strslot1, 0, sizeof(strslot1)); intslot1=0; intslot2=0; intslot3=0; intslot4=0;
    }
    for(i=1; i<(UNIT_CAP+1) ;i++)
    {
        JsonBitirici("Files/unit_types.json", -1, i, strslot1, 0, 0, 0, 0, 0, "saldiri", &intslot1, "savunma", &intslot2, "saglik", &intslot3, "kritik_sans", &intslot4, *insanimp, *orklegi, 0, 0, 0);
        strcpy(orklegi->unit[i-1].name, strslot1);
        orklegi->unit[i-1].atk=intslot1;
        orklegi->unit[i-1].def=intslot2;
        orklegi->unit[i-1].hp=intslot3;
        orklegi->unit[i-1].crit=intslot4;
        memset(strslot1, 0, sizeof(strslot1)); intslot1=0; intslot2=0; intslot3=0; intslot4=0;
    }
}

void YaratiklariYedim(struct Faction *insanimp, struct Faction *orklegi)
{
    char strslot1[OBJ_NAME_LEN]={0}, strslot2[OBJ_NAME_LEN]={0}, strslot3[OBJ_NAME_LEN]={0}, strslot4[OBJ_NAME_LEN]={0};
    int i=0;
    for(i=1; i<(CREATURE_CAP+1) ;i++)
    {
        JsonBitirici("Files/creatures.json", 1, i, strslot1, 0 ,"etki_degeri", strslot2, "etki_turu", strslot3, 0, 0, 0, 0, 0, 0, 0, 0, *insanimp, *orklegi, 3, strslot4, 0);
        strcpy(insanimp->creature[i-1].name, strslot1);
        strcpy(insanimp->creature[i-1].effecttype, strslot3);
        strcpy(insanimp->creature[i-1].targetunit, strslot4);
        insanimp->creature[i-1].effectvalue=atoi(strslot2);
        memset(strslot1, 0, sizeof(strslot1)); memset(strslot2, 0, sizeof(strslot2)); memset(strslot3, 0, sizeof(strslot3)); memset(strslot4, 0, sizeof(strslot4));
    }
    for(i=1; i<(CREATURE_CAP+1) ;i++)
    {
        JsonBitirici("Files/creatures.json", -1, i, strslot1, 0 ,"etki_degeri", strslot2, "etki_turu", strslot3, 0, 0, 0, 0, 0, 0, 0, 0, *insanimp, *orklegi, 3, strslot4, 0);
        strcpy(orklegi->creature[i-1].name, strslot1);
        strcpy(orklegi->creature[i-1].effecttype, strslot3);
        strcpy(orklegi->creature[i-1].targetunit, strslot4);
        orklegi->creature[i-1].effectvalue=atoi(strslot2);
        memset(strslot1, 0, sizeof(strslot1)); memset(strslot2, 0, sizeof(strslot2)); memset(strslot3, 0, sizeof(strslot3)); memset(strslot4, 0, sizeof(strslot4));
    }
}

void KahramanlariYedim(struct Faction *insanimp, struct Faction *orklegi)
{
    char strslot1[OBJ_NAME_LEN]={0}, strslot2[OBJ_NAME_LEN]={0}, strslot3[OBJ_NAME_LEN]={0}, strslot4[OBJ_NAME_LEN]={0};
    int i=0;
    for(i=1; i<(HERO_CAP+1) ;i++)
    {
        JsonBitirici("Files/heroes.json", 1, i, strslot1, 0, "bonus_turu", strslot2, "bonus_degeri", strslot3, 0, 0, 0, 0, 0, 0, 0, 0, *insanimp, *orklegi, 3, strslot4, 0);
        strcpy(insanimp->hero[i-1].name, strslot1);
        strcpy(insanimp->hero[i-1].bonustype, strslot2);
        strcpy(insanimp->hero[i-1].targetunit, strslot4);
        insanimp->hero[i-1].bonusvalue=atoi(strslot3);
        memset(strslot1, 0, sizeof(strslot1)); memset(strslot2, 0, sizeof(strslot2)); memset(strslot3, 0, sizeof(strslot3)); memset(strslot4, 0, sizeof(strslot4));
    }
    for(i=1; i<(HERO_CAP+1) ;i++)
    {
        JsonBitirici("Files/heroes.json", -1, i, strslot1, 0, "bonus_turu", strslot2, "bonus_degeri", strslot3, 0, 0, 0, 0, 0, 0, 0, 0, *insanimp, *orklegi, 3, strslot4, 0);
        strcpy(orklegi->hero[i-1].name, strslot1);
        strcpy(orklegi->hero[i-1].bonustype, strslot2);
        strcpy(orklegi->hero[i-1].targetunit, strslot4);
        orklegi->hero[i-1].bonusvalue=atoi(strslot3);
        memset(strslot1, 0, sizeof(strslot1)); memset(strslot2, 0, sizeof(strslot2)); memset(strslot3, 0, sizeof(strslot3)); memset(strslot4, 0, sizeof(strslot4));
    }
}

void ArastirmalariYedim(struct Research *research, struct Faction *insanimp, struct Faction *orklegi)
{
    char strslot1[OBJ_NAME_LEN]={0}, strslot2[OBJ_NAME_LEN]={0}, strslot3[OBJ_NAME_LEN]={0}, strslot4[OBJ_NAME_LEN]={0}, strslot5[OBJ_NAME_LEN];
    int i=0, j=0;
    for(i=1; i<RESEARCH_CAP/3 ;i++)
    {
        for(j=1; j<4 ;j++)
        {
            JsonBitirici("Files/research.json", 10+i, j, strslot1, strslot2, "deger", strslot3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, *insanimp, *orklegi, 1, strslot4, strslot5);
            strcpy(research[3*i-3+j-1].name, strslot1);
            strcpy(research[3*i-3+j-1].upgradetype, strslot5);
            strcpy(research[3*i-3+j-1].targetunit, strslot4);
            research[3*i-3+j-1].upgradevalue=atoi(strslot3);
            research[3*i-3+j-1].level = atoi(1+strchr(strslot2, '_'));
            memset(strslot1, 0, sizeof(strslot1)); memset(strslot2, 0, sizeof(strslot2)); memset(strslot3, 0, sizeof(strslot3)); memset(strslot4, 0, sizeof(strslot4)); memset(strslot4, 0, sizeof(strslot5));
        }
    }
}

void JsonBitirici(const char *filepath, const short factionno, const short layer3pick, char *assigname1, char *assigname2, const char *expect1, char *assign1, const char *expect2, char *assign2, const char *intexpect1, int *intassign1, const char *intexpect2, int *intassign2, const char *intexpect3, int *intassign3, const char *intexpect4, int *intassign4, const struct Faction insanlartamliste, const struct Faction orklartamliste, const short aciklamacekis1, char *acikassign1, char *acikassign2)
{
    short finalstroke=0;            // stroke fonksiyonunun son kaldigi yeri tutar
    short currentname=0;            // kacinci 3. derinlik elemaninda oldugumuzu belirtir (anlamadiysan sorun yok bende anlamadim)
    short currentfact=0;            // 1 ise insanlarin tarafi okunuyordur, -1 ise orklarin tarafi okunuyordur, 10 ve ustuyse researchi okuyoruzdur (sorma)
    short bracedepth=0;             // kac parantez derinliginde oldugumuzu belirtir
    char holdIT[OBJ_NAME_LEN];      // cektigimiz kelime
    char line[LINE_LEN];            // cektigimiz satir
    FILE *json = fopen(filepath,"r");
    if(factionno>10) currentfact=10;
    if(json == NULL){printf("Dosya \"%s\" acilamadi, lutfen Files klasorunun icinde oldugundan emin olunuz.\n", filepath); return;}
    for(short i=0; fgetc(json)!=EOF ;i++)
    {
        fseek(json, -1, SEEK_CUR);  // for loop fgetc kullanarak dosyanin bitmedigini anliyor, ama fgetc dosya pointerini ilerletiyor, biz de geriletiyoruz
        memset(line, 0, sizeof(line));
        fgets(line, LINE_LEN, json);    // bir sonraki satiri okuyup line'a aktarir
        finalstroke = stroke(line, 0, holdIT);  // strtok ise yaramaz fonksiyonun teki o yuzden bunu kullaniyoz
        if(bracedepth==1 && strcmp(holdIT, "insan_imparatorlugu")==0)
        { // insan imp okunursa currentfact 1 olur
            currentfact=1;
            currentname=0;
        }
        if(bracedepth==1 && strcmp(holdIT, "ork_legi")==0)
        { // ork legi okunursa currentfact -1 olur
            currentfact=-1;
            currentname=0;
        }
        if(bracedepth==1 && factionno>10)
        { // factionno 11 = 1 numarali arastirma // factionno 12 = 2 numarali arastirma
            currentfact++;
            if(currentfact==factionno) strcpy(assigname1, holdIT);
        }
        if(bracedepth==2 && currentfact==factionno)  currentname++; //derinlik 2 oldugunda bir sonraki isme gectigimizi varsayar (nizami bir denge, ben anlatamam)
        if(bracedepth==2 && currentfact==factionno && currentname==layer3pick && factionno<10) strcpy(assigname1, holdIT); //research harici isim atamasi
        if(assigname2!=NULL && bracedepth==2 && currentname==layer3pick) strcpy(assigname2, holdIT); // assigname2 istenildiyse assigname2 atiyor (research icin, sorgulama)
        if(expect1!=NULL && bracedepth==3 && currentfact==factionno && currentname==layer3pick && 0==strcmp(holdIT, expect1))
        { // 1 numarali yer isteniyorsa stringe bakip atamaya calisir
            memset(holdIT, 0, sizeof(holdIT));
            stroke(line, finalstroke+1, holdIT);
            strcpy(assign1, holdIT);
        }
        if(expect2!=NULL && bracedepth==3 && currentfact==factionno && currentname==layer3pick && 0==strcmp(holdIT, expect2))
        { // 2 numarali yer isteniyorsa stringe bakip atamaya calisir
            memset(holdIT, 0, sizeof(holdIT));
            stroke(line, finalstroke+1, holdIT);
            strcpy(assign2, holdIT);
        }
        if(intexpect1!=NULL && bracedepth==3 && currentfact==factionno && currentname==layer3pick && 0==strcmp(holdIT, intexpect1))
        { // 1 numarali integer yeri isteniyorsa stringe bakip atamaya calisir
            strcpy(line, strrchr(line, ' '));
            *intassign1 = atoi(line);
        }
        if(intexpect2!=NULL && bracedepth==3 && currentfact==factionno && currentname==layer3pick && 0==strcmp(holdIT, intexpect2))
        { // 2 numarali integer yeri isteniyorsa stringe bakip atamaya calisir
            strcpy(line, strrchr(line, ' '));
            *intassign2 = atoi(line);
        }
        if(intexpect3!=NULL && bracedepth==3 && currentfact==factionno && currentname==layer3pick && 0==strcmp(holdIT, intexpect3))
        { // 3 numarali integer yeri isteniyorsa stringe bakip atamaya calisir
            strcpy(line, strrchr(line, ' '));
            *intassign3 = atoi(line);
        }
        if(intexpect4!=NULL && bracedepth==3 && currentfact==factionno && currentname==layer3pick && 0==strcmp(holdIT, intexpect4))
        { // 4 numarali integer yeri isteniyorsa stringe bakip atamaya calisir
            strcpy(line, strrchr(line, ' '));
            *intassign4 = atoi(line);
        } // aciklamacekis=1 ise arastirmalara bakiyoruzdur (AAAAAAAAAAAAAAAAAAAAAAAA)
        if(aciklamacekis1==1 && bracedepth==3 && currentname==layer3pick && 0==strcmp(holdIT, "aciklama") && currentfact==factionno)
        {
            stroke(line, finalstroke+1, holdIT);
            if(strstr(holdIT, "tum_birimler")) strcpy(acikassign1, "tum_birimler");
            for(short i=0; i<UNIT_CAP ;i++) if(strstr(holdIT, insanlartamliste.unit[i].name) && 1<strlen(insanlartamliste.unit[i].name)) strcpy(acikassign1, insanlartamliste.unit[i].name);
            // yukaridaki kod butun insan unitlerini aciklamada ariyor, bulursa atama gerceklestiriyor
            for(short i=0; i<UNIT_CAP ;i++) if(strstr(holdIT, orklartamliste.unit[i].name) && 1<strlen(orklartamliste.unit[i].name)) strcpy(acikassign1, orklartamliste.unit[i].name);
            // yukaridaki kod butun ork unitlerini aciklamada ariyor, bulursa atama gerceklestiriyor
            if(strstr(holdIT, "saldiri")) strcpy(acikassign2, "saldiri");
            if(strstr(holdIT, "savunma")) strcpy(acikassign2, "savunma");
            if(strstr(holdIT, "kritik")) strcpy(acikassign2, "kritik_sans");
            if(strstr(holdIT, "saglik")) strcpy(acikassign2, "saglik");
            // yukaridaki kod arastirmanin verdigi bonusu atiyor (ilk defa anlasilir bir sey)
        }

        //aciklamacekis=3 means it detecs unit type
        if(aciklamacekis1==3 && bracedepth==3 && currentname==layer3pick && 0==strcmp(holdIT, "aciklama") && currentfact==factionno)
        {
            stroke(line, finalstroke+1, holdIT);
            if(strstr(holdIT, "tum_birimler")) strcpy(acikassign1, "tum_birimler");
            for(short i=0; i<UNIT_CAP ;i++) if(strstr(holdIT, insanlartamliste.unit[i].name) && 1<strlen(insanlartamliste.unit[i].name)) strcpy(acikassign1, insanlartamliste.unit[i].name);
            // yukaridaki kod butun insan unitlerini aciklamada ariyor, bulursa atama gerceklestiriyor
            for(short i=0; i<UNIT_CAP ;i++) if(strstr(holdIT, orklartamliste.unit[i].name) && 1<strlen(orklartamliste.unit[i].name)) strcpy(acikassign1, orklartamliste.unit[i].name);
            // yukaridaki kod butun ork unitlerini aciklamada ariyor, bulursa atama gerceklestiriyor
        }
        if(strchr(line, '{')!=NULL) bracedepth++; // bu iki fonksiyon parantez derinligini ayarliyor
        if(strchr(line, '}')!=NULL) bracedepth--;
        memset(holdIT, 0, sizeof(holdIT)); // bu da tuttugumuz parcayi sifirliyor
    }
    fclose(json);
    return;
};

short stroke(char line[LINE_LEN], short startpos, char returnage[OBJ_NAME_LEN])
{
    short i, s;
    for(i=startpos; line[i]!='\0' && line[i]!='"' ;i++){} //stops at the first quote
    if(line[i]!='\0')i++; //skips the quote and directly onto the string we need
    for(s=0; line[i]!='\0' && line[i]!='"' ;i++, s++) returnage[s] = line[i]; //copies everything until the next quote
    return i;
}

void SenaryoyuYedim(const char *filepath, struct Senaryo *ordu, const struct Faction Devlet, const struct Research Arastirma[RESEARCH_CAP], const short whichside)
{
    char scenario[JSON_LEN] = {0};
    char *strpos;
    int insanfoundat=0;
    int orkfoundat=0;
    FILE *json = fopen(filepath,"r");
    if(json == NULL){printf("Dosya \"%s\" acilamadi, lutfen Files klasorunun icinde oldugundan emin olunuz.\n", filepath); return;}
    fread(scenario, 1, 2048, json);
    fclose(json);
    for(short i=0; i<HERO_CAP ;i++)     if(strstr(scenario, Devlet.hero[i].name) && 1<strlen(Devlet.hero[i].name))     strcpy(ordu->Hero_name, Devlet.hero[i].name);
    for(short i=0; i<CREATURE_CAP ;i++) if(strstr(scenario, Devlet.creature[i].name) && 1<strlen(Devlet.creature[i].name)) strcpy(ordu->creature_name, Devlet.creature[i].name);

    for(short i=0; i<UNIT_CAP ;i++)
    {
        if((strpos=strstr(scenario, Devlet.unit[i].name)) && 1<strlen(Devlet.unit[i].name))
        {
            strcpy(ordu->CU[i].unit.name, Devlet.unit[i].name);
            strpos = strchr(strpos, ' '); ordu->CU[i].sayi = atoi(strpos+1);
        }
    }
    strpos = strstr(scenario, "insan_imparatorlugu"); insanfoundat = strpos - scenario;
    strpos = strstr(scenario, "ork_legi"); orkfoundat = strpos - scenario;
    for(short i=0; i<RESEARCH_CAP ;i++)
    {
        if((strpos=strstr(scenario, Arastirma[i].name)) && 1<strlen(Arastirma[i].name))
        {
            if((whichside==1 && (((strpos-scenario)>insanfoundat && (strpos-scenario)<orkfoundat) || ((strpos-scenario)>insanfoundat && (strpos-scenario)>orkfoundat && orkfoundat<insanfoundat))) || (whichside==-1 && (((strpos-scenario)>orkfoundat && (strpos-scenario)<insanfoundat) || ((strpos-scenario)>orkfoundat && (strpos-scenario)>insanfoundat && insanfoundat<orkfoundat))))
            {
                strcpy(ordu->research_name, Arastirma[i].name);
                strpos = strchr(strpos, ' '); ordu->res_lvl = atoi(strpos+1);
            }
        }
    }
}

void Givestat(struct Faction *insanimp, struct Faction *orglegi, struct Senaryo *insanordusu, struct Senaryo *orkordusu)
{
    for(int i = 0; i < UNIT_CAP ; i++)
    {
        for(int j = 0 ; j < UNIT_CAP ; j++)
        {
            if(strcmp(insanordusu->CU[i].unit.name,insanimp->unit[j].name) == 0)
            {
                insanordusu->CU[i].unit.atk = insanimp->unit[j].atk;
                insanordusu->CU[i].unit.def = insanimp->unit[j].def;
                insanordusu->CU[i].unit.crit = insanimp->unit[j].crit;
                insanordusu->CU[i].unit.hp = insanimp->unit[j].hp;
            }
        }
    }
    for(int i = 0; i < UNIT_CAP ; i++)
    {
        for(int j = 0 ; j < UNIT_CAP ; j++)
        {
            if(strcmp(orkordusu->CU[i].unit.name,orglegi->unit[j].name) == 0)
            {
                orkordusu->CU[i].unit.atk = orglegi->unit[j].atk;
                orkordusu->CU[i].unit.def = orglegi->unit[j].def;
                orkordusu->CU[i].unit.crit = orglegi->unit[j].crit;
                orkordusu->CU[i].unit.hp = orglegi->unit[j].hp;
            }
        }
    }
}

void BuffsAndNerfs(struct Faction *insanimp, struct Faction *orglegi, struct Senaryo *insanordusu, struct Senaryo *orkordusu ,struct Research *Research)
{
    for(int i = 0 ; i < UNIT_CAP ; i++)
    {
        if(strcmp(insanordusu->creature_name,insanimp->creature[i].name)== 0)
        {
            for(int j = 0 ; j < UNIT_CAP ; j++)
            {
                if(strcmp(insanordusu->CU[j].unit.name , insanimp->creature[i].targetunit) == 0)
                {
                    if(strcmp(insanimp->creature[i].effecttype ,"saldiri") == 0)
                        insanordusu->CU[j].unit.atk = (insanordusu->CU[j].unit.atk * (100 + insanimp->creature[i].effectvalue))/100;
                    else if(strcmp(insanimp->creature[i].effecttype,"savunma") == 0)
                        insanordusu->CU[j].unit.def = (insanordusu->CU[j].unit.def * (100 + insanimp->creature[i].effectvalue))/100;
                    else if(strcmp(insanimp->creature[i].effecttype, "kritik_sans") == 0)
                        insanordusu->CU[j].unit.crit += insanimp->creature[i].effectvalue;
                }
            }
        }
    }

    for(int i = 0 ; i < UNIT_CAP ; i++)
    {
        if(strcmp(orkordusu->creature_name, orglegi->creature[i].name) == 0)
        {
            for(int j = 0 ; j < UNIT_CAP ; j++)
            {
                if(strcmp(orkordusu->CU[j].unit.name , orglegi->creature[i].targetunit) == 0)
                {
                    if(strcmp(orglegi->creature[i].effecttype , "saldiri") == 0)
                        orkordusu->CU[j].unit.atk = (orkordusu->CU[j].unit.atk * (100 + orglegi->creature[i].effectvalue))/100;
                    else if(strcmp(orglegi->creature[i].effecttype , "savunma") == 0)
                        orkordusu->CU[j].unit.def = (orkordusu->CU[j].unit.def * (100 + orglegi->creature[i].effectvalue))/100;
                    else if(strcmp( orglegi->creature[i].effecttype , "kritik_sans") == 0)
                        orkordusu->CU[j].unit.crit += orglegi->creature[i].effectvalue;
                }
            }
        }
    }
    for(int i = 0 ; i < UNIT_CAP ; i++)
    {
        if(strcmp(insanordusu->Hero_name ,insanimp->hero[i].name) == 0)
        {
            for(int j = 0 ; j < UNIT_CAP ; j++)
            {
                if(strcmp(insanordusu->CU[j].unit.name, insanimp->hero[i].targetunit) == 0)
                {
                    if(strcmp(insanimp->hero[i].bonustype , "saldiri") == 0)
                        insanordusu->CU[j].unit.atk = (insanordusu->CU[j].unit.atk * (100 + insanimp->hero[i].bonusvalue))/100;
                    else if(strcmp(insanimp->hero[i].bonustype , "savunma") == 0)
                        insanordusu->CU[j].unit.def = (insanordusu->CU[j].unit.def * (100 + insanimp->hero[i].bonusvalue))/100;
                    else if(strcmp(insanimp->hero[i].bonustype , "kritik_sans") == 0)
                        insanordusu->CU[j].unit.crit += insanimp->hero[i].bonusvalue;
                }
                else if(strcmp("tum_birimlere" ,insanimp->hero[i].targetunit) == 0)
                {
                    for(int x = 0 ; x < UNIT_CAP ; x++)
                    {
                        if(strcmp(insanimp->hero[i].bonustype , "saldiri") == 0)
                            insanordusu->CU[x].unit.atk = (insanordusu->CU[x].unit.atk * (100 + insanimp->hero[i].bonusvalue))/100;
                        else if(strcmp(orglegi->hero[i].bonustype , "savunma") == 0)
                            insanordusu->CU[x].unit.def = (insanordusu->CU[x].unit.def * (100 + insanimp->hero[i].bonusvalue))/100;
                        else if(strcmp(orglegi->hero[i].bonustype, "kritik_sans") == 0)
                            insanordusu->CU[x].unit.crit += insanimp->hero[i].bonusvalue;
                    }
                }
            }
        }
    }
    for(int i = 0 ; i < UNIT_CAP ; i++)
    {
        if(strcmp(orkordusu->Hero_name , orglegi->hero[i].name) == 0)
        {
            for(int j = 0 ; j < UNIT_CAP ; j++)
            {
                if(strcmp(orkordusu->CU[j].unit.name ,orglegi->hero[i].targetunit) == 0)
                {
                    if(strcmp(orglegi->hero[i].bonustype , "saldiri") == 0)
                        orkordusu->CU[j].unit.atk = (orkordusu->CU[j].unit.atk * (100 + orglegi->hero[i].bonusvalue))/100;
                    else if(strcmp(orglegi->hero[i].bonustype , "savunma") == 0)
                        orkordusu->CU[j].unit.def = (orkordusu->CU[j].unit.def * (100 + orglegi->hero[i].bonusvalue))/100;
                    else if(strcmp(orglegi->hero[i].bonustype, "kritik_sans") == 0)
                        orkordusu->CU[j].unit.crit += orglegi->hero[i].bonusvalue;
                }
                else if(strcmp("tum_birimlere" ,orglegi->hero[i].targetunit) == 0)
                {
                    for(int x = 0 ; x < UNIT_CAP ; x++)
                    {
                        if(strcmp(orglegi->hero[i].bonustype , "saldiri") == 0)
                            orkordusu->CU[x].unit.atk = (orkordusu->CU[x].unit.atk * (100 + orglegi->hero[i].bonusvalue))/100;
                        else if(strcmp(orglegi->hero[i].bonustype , "savunma") == 0)
                            orkordusu->CU[x].unit.def = (orkordusu->CU[x].unit.def * (100 + orglegi->hero[i].bonusvalue))/100;
                        else if(strcmp(orglegi->hero[i].bonustype, "kritik_sans") == 0)
                            orkordusu->CU[x].unit.crit += orglegi->hero[i].bonusvalue;
                    }
                }
            }
        }
    }
    for(int i = 0 ; i < RESEARCH_CAP ; i++)
    {
        if((strcmp(insanordusu->research_name,Research[i].name) == 0) && (Research[i].level == insanordusu->res_lvl))
        {
            for(int j = 0 ; j < UNIT_CAP ; j++)
            {
                if(strcmp(Research[i].targetunit,insanordusu->CU[j].unit.name) == 0)
                {
                    if(strcmp(Research[i].upgradetype,"saldiri") == 0)
                        insanordusu->CU[j].unit.atk = (insanordusu->CU[j].unit.atk * (100 + Research[i].upgradevalue))/100;
                    else if(strcmp(Research[i].upgradetype,"savunma") == 0)
                        insanordusu->CU[j].unit.def = (insanordusu->CU[j].unit.def * (100 + Research[i].upgradevalue))/100;
                    else if(strcmp(Research[i].upgradetype,"kritik_vuruþ") == 0)
                        insanordusu->CU[j].unit.crit += Research[i].upgradevalue;
                }

                else if(strcmp(Research[i].targetunit,"tum_birimler") ==0)
                {
                        if(strcmp(Research[i].upgradetype,"saldiri") == 0)
                            insanordusu->CU[j].unit.atk = (insanordusu->CU[j].unit.atk * (100 + Research[i].upgradevalue))/100;
                        else if(strcmp(Research[i].upgradetype,"savunma") == 0)
                            insanordusu->CU[j].unit.def = (insanordusu->CU[j].unit.def * (100 + Research[i].upgradevalue))/100;
                        else if(strcmp(Research[i].upgradetype,"kritik_vuruþ") == 0)
                            insanordusu->CU[j].unit.crit += Research[i].upgradevalue;
                }
            }
        }
    }
    for(int i = 0 ; i < RESEARCH_CAP ; i++)
    {
        if(strcmp(orkordusu->research_name,Research[i].name) == 0 && Research[i].level == orkordusu->res_lvl)
        {
            for(int j = 0 ; j < UNIT_CAP ; j++)
            {
                if(strcmp(Research[i].targetunit,orkordusu->CU[j].unit.name) == 0)
                {
                    if(strcmp(Research[i].upgradetype,"saldiri") == 0)
                        orkordusu->CU[j].unit.atk = (orkordusu->CU[j].unit.atk * (100 + Research[i].upgradevalue))/100;
                    else if(strcmp(Research[i].upgradetype,"savunma") == 0)
                        orkordusu->CU[j].unit.def = (orkordusu->CU[j].unit.def * (100 + Research[i].upgradevalue))/100;
                    else if(strcmp(Research[i].upgradetype,"kritik_vuruþ") == 0)
                        orkordusu->CU[j].unit.crit += Research[i].upgradevalue;
                }
                else if(strcmp(Research[i].targetunit,"tum_birimler") ==0)
                {
                        if(strcmp(Research[i].upgradetype,"saldiri") == 0)
                            orkordusu->CU[j].unit.atk = (orkordusu->CU[j].unit.atk * (100 + Research[i].upgradevalue))/100;
                        else if(strcmp(Research[i].upgradetype,"savunma") == 0)
                            orkordusu->CU[j].unit.def = (orkordusu->CU[j].unit.def * (100 + Research[i].upgradevalue))/100;
                        else if(strcmp(Research[i].upgradetype,"kritik_vuruþ") == 0)
                            orkordusu->CU[j].unit.crit += Research[i].upgradevalue;
                }
            }
        }
    }
}

int Simulation(struct Senaryo *insanordusu,struct Senaryo *orkordusu)
{
    FILE *file = fopen("savas_sim.txt","w");
    int Turn_number = 1;
    while(IsBattleOver(insanordusu) != 0 || IsBattleOver(orkordusu) != 0)
    {
        if(Turn_number%2 != 0 || Turn_number == 1)     //////////////////// INSAN KISMI ///////////////////////////
        {
            int Attack_turn_for_humans = 1 ;
            CalculateFatigue(insanordusu,Turn_number);
            CalculateFatigue(orkordusu,Turn_number);
            int damage = CalculateDamage(insanordusu,Attack_turn_for_humans);
            int def = CalculateDefense(orkordusu);

            //Insanlarin statlari
            fprintf(file,"******************INSANLARIN SIRASI**************\n");
            for(int i = 0 ; i < UNIT_CAP ; i++)
                fprintf(file,"%s :  %d %d %d %d %d\n",insanordusu->CU[i].unit.name,insanordusu->CU[i].sayi,insanordusu->CU[i].unit.atk,insanordusu->CU[i].unit.crit,insanordusu->CU[i].unit.def,insanordusu->CU[i].unit.hp);
            fprintf(file,"\nInsanlarin atagi : %d\n",damage);
            fprintf(file,"Insanlarin defansi : %d\n\n\n",CalculateDefense(insanordusu));

            //Orglarin statlari
            for(int i = 0 ; i < UNIT_CAP ; i++)
                fprintf(file,"%s :  %d %d %d %d %d\n",orkordusu->CU[i].unit.name,orkordusu->CU[i].sayi,orkordusu->CU[i].unit.atk,orkordusu->CU[i].unit.crit,orkordusu->CU[i].unit.def,orkordusu->CU[i].unit.hp);
            fprintf(file,"\nOrklarin atagi : %d\n",CalculateDamage(orkordusu,Attack_turn_for_humans));
            fprintf(file,"Orklarin defansi : %d\n",CalculateDefense(orkordusu));

            if(IsBattleOver(insanordusu) == 0)
                break;

            int TrueDamage = damage * (1 - ((float)(def)/(float)(damage)));
            if(TrueDamage <= 0)
                TrueDamage = 0;
            fprintf(file,"\n\n\n\nAsil vurulacak hasar: %d\n\n\n",TrueDamage);

            for(int i = 0 ; i < UNIT_CAP ; i++){
                orkordusu->CU[i].unit.hp -= TrueDamage*((float)(orkordusu->CU[i].unit.def)/(float)(def));

                if(orkordusu->CU[i].unit.hp <= 0 )
                {
                    orkordusu->CU[i].sayi = 0;
                    orkordusu->CU[i].unit.hp = 0;
                }
            }
            fprintf(file,"**************INSANLAR  SALDIRDI*******************\n\n");
            for(int i = 0 ; i < UNIT_CAP ; i++)
                fprintf(file,"%s :  %d %d %d %d %d\n",orkordusu->CU[i].unit.name,orkordusu->CU[i].sayi,orkordusu->CU[i].unit.atk,orkordusu->CU[i].unit.crit,orkordusu->CU[i].unit.def,orkordusu->CU[i].unit.hp);

            fprintf(file,"\n\n*************TUR BITTI**************\n\n");
            Attack_turn_for_humans++;
        }
        else                                /////////////////////////////ORG KISMI//////////////////////////
        {
            int Attack_turn_for_orgs = 1 ;

            CalculateFatigue(orkordusu,Turn_number);
            CalculateFatigue(insanordusu,Turn_number);
            int damage = CalculateDamage(orkordusu,Attack_turn_for_orgs);
            int def = CalculateDefense(insanordusu);

            //ORGLARIN STATLARI
            fprintf(file,"******************ORGLARIN SIRASI**************\n");
            for(int i = 0 ; i < UNIT_CAP ; i++)
                fprintf(file,"%s :  %d %d %d %d %d\n",orkordusu->CU[i].unit.name,orkordusu->CU[i].sayi,orkordusu->CU[i].unit.atk,orkordusu->CU[i].unit.crit,orkordusu->CU[i].unit.def,orkordusu->CU[i].unit.hp);
            fprintf(file,"\nOrklarin atagi : %d\n",CalculateDamage(orkordusu,Attack_turn_for_orgs));
            fprintf(file,"Orklarin defansi : %d\n",CalculateDefense(orkordusu));

            //INSANLARIN STATLARI
            for(int i = 0 ; i < UNIT_CAP ; i++)
                fprintf(file,"%s :  %d %d %d %d %d\n",insanordusu->CU[i].unit.name,insanordusu->CU[i].sayi,insanordusu->CU[i].unit.atk,insanordusu->CU[i].unit.crit,insanordusu->CU[i].unit.def,insanordusu->CU[i].unit.hp);

            fprintf(file,"\nInsanlarin atagi : %d\n",CalculateDamage(insanordusu,Attack_turn_for_orgs));
            fprintf(file,"Insanlarin defansi : %d\n\n\n",CalculateDefense(insanordusu));

            if(IsBattleOver(orkordusu) == 0)
                break;

            int TrueDamage = damage * (1-((float)(def)/(float)(damage)));
            if(TrueDamage <= 0)
                TrueDamage = 0;
            fprintf(file,"\n\n\n\nAsil vurulacak hasar: %d\n\n\n",TrueDamage);

            for(int i = 0 ; i < UNIT_CAP ; i++){
                insanordusu->CU[i].unit.hp -= TrueDamage*((float)(insanordusu->CU[i].unit.def)/(float)(def));

                if(insanordusu->CU[i].unit.hp < 0 )
                {
                    insanordusu->CU[i].sayi = 0;
                    insanordusu->CU[i].unit.hp = 0;
                }
            }
            fprintf(file,"**************ORGLAR SALDIRDI*******************\n\n");
            for(int i = 0 ; i < UNIT_CAP ; i++)
                fprintf(file,"%s :  %d %d %d %d %d\n",insanordusu->CU[i].unit.name,insanordusu->CU[i].sayi,insanordusu->CU[i].unit.atk,insanordusu->CU[i].unit.crit,insanordusu->CU[i].unit.def,insanordusu->CU[i].unit.hp);
            fprintf(file,"\n\n*************TUR BITTI**************\n\n");

            Attack_turn_for_orgs++;
        }
        Turn_number++;
        //printf("\n\nTur sayisi : %d\n\n",Turn_number-1);
    }

    if(IsBattleOver(insanordusu) == 0)
    {
        fprintf(file,"\n\n\nOrglar Kazandi !!!!!");
        fclose(file);
        return -1;
    }
    else if(IsBattleOver(orkordusu) == 0)
    {
        fprintf(file,"\n\n\nInsanlar Kazandi !!!!!");
        fclose(file);
        return 1;
    }
    return 0;
}

int IsBattleOver(struct Senaryo *GoofyAhhStruct)
{
    int check = 5;
    for(int i = 0 ; i < UNIT_CAP ; i++)
    {
        if(GoofyAhhStruct->CU[i].sayi <= 0)
            check--;
    }
    return check;
}

int CalculateDamage(struct Senaryo *GoofyAhhStruct,int tur)
{
    int total_damage = 0;
    for(int i = 0 ; i < UNIT_CAP ; i++)
    {
        total_damage += GoofyAhhStruct->CU[i].sayi*GoofyAhhStruct->CU[i].unit.atk*CalculateCrit(GoofyAhhStruct,tur,i);
    }
    return total_damage;
}

float CalculateCrit(struct Senaryo *GoofyAhhStruct,int Tur,int number)
{
    int i = 1;
    if(GoofyAhhStruct->CU[number].unit.crit != 0)
    {
        for(; (GoofyAhhStruct->CU[number].unit.crit*i) < 100 ;i++) {}
    }
    else return 1;
    if(Tur%i == 0 && Tur >= i)
    {
        return 1.5;
    }
    return 1;
}

int CalculateDefense(struct Senaryo *GoofyAhhStruct)
{
    int total_defense = 0;
    for(int i = 0 ; i < UNIT_CAP ; i++){
        total_defense += GoofyAhhStruct->CU[i].unit.def * GoofyAhhStruct->CU[i].sayi;
    }
    return total_defense;
}

void CalculateFatigue(struct Senaryo *GoofyAhhStruct,int tur)
{
    if(tur%5 == 1 && tur != 1)
    {
        for(int i = 0 ; i < UNIT_CAP ; i++)
        {
            GoofyAhhStruct->CU[i].unit.atk = (GoofyAhhStruct->CU[i].unit.atk * 90)/100;
            GoofyAhhStruct->CU[i].unit.def = (GoofyAhhStruct->CU[i].unit.def * 90)/100;
        }
    }
}

void WriteJSON(int x)
{

    char url[100];
    char Name[100];

    sprintf(url,"https://yapbenzet.org.tr/%d.json",x);
    sprintf(Name,"Files\\%d.json",x);

    FILE* file =fopen(Name,"w");
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();


    curl_easy_setopt(curl, CURLOPT_CAINFO, "Certificate\\cacert.pem");
    curl_easy_setopt(curl, CURLOPT_CAPATH, "Certificate\\cacert.pem");

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetSizeJSON);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

    fclose(file);
    curl_easy_cleanup(curl);
  }
}

size_t GetSizeJSON(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}
