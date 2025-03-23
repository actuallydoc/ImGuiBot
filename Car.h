// Created by Maj Knez on 21. 3. 25.
//

#ifndef CAR_H
#define CAR_H

#define BUFFER_SIZE 128

class Car {
public:
    // Basic information
    const char* gorivo[3] = {"Elektrika", "Diesel", "Bencin"};
    int selected_gorivo = 0;
    const char* karoserijska_oblika[3] = { "Limuzina", "SUV", "Coupe" };
    int selected_karoserijska_oblika = 0;
    char znamka_buffer[BUFFER_SIZE] = {};
    char model_buffer[BUFFER_SIZE] = {};
    char model_vozila_buffer[BUFFER_SIZE] = {};
    char znamka_vozila_buffer[BUFFER_SIZE] = {};
    char letnik_registracije_buffer[BUFFER_SIZE] = {};
    char mesec_registracije_buffer[BUFFER_SIZE] = {};
    char index_to_select[BUFFER_SIZE] = {};

    // Information
    char vin_buffer[BUFFER_SIZE] = {};
    bool objavi_vin = false;

    // Additional fields
    const char* menjalnikOptions[2] = { "rocni", "avtomatski" };
    int selected_menjalnik = 0;
    char letoReg[BUFFER_SIZE] = "2020";
    char prevozenikm[BUFFER_SIZE] = "30000";
    char tehLeto[BUFFER_SIZE] = "2023";
    char najem[BUFFER_SIZE] = "1";
    char cena[BUFFER_SIZE] = "500";
    bool gotpopust = true;
    bool zadcena = true;
    bool prevzemleas = true;
    bool menjava = true;
    bool prodamvceloti = true;
    bool vozno = false;
    bool poskodovan = true;
    bool karambolirano = true;
    bool okvara = false;
    bool poplavljeno = false;
    bool SK = false;
    bool SKpotrjena = false;
    bool SLOporeklo = false;
    bool gar = false;
    bool nek = false;
    const char* barve[5] = {"bela", "crna", "siva", "modra", "rdeca"};
    int selected_barva = 0;
    bool kovinska = true;
    bool elstreha = false;
    bool dostvrat04 = false;
    const char* sedezev_options[3] = {"2", "4", "5"};
    int selected_sedezev = 2;
    char porabaKombi[BUFFER_SIZE] = "6,3";
    bool ALU = true;
    char ALUopis[BUFFER_SIZE] = "lita";
    bool ABSzavore = true;
    bool BAS = true;
    bool ASD = false;
    bool ESP = true;
    bool EDC = false;
    bool nivopodvozja = false;
    bool FVol4WS = false;
    bool ASR = true;
    bool ETS = true;
    bool sportnopodvozje = false;
    bool ABC = false;
    bool FAirVzm = false;
    char airbagov[BUFFER_SIZE] = "6";
    bool nadzortlaka = true;
    bool xenon = false;
    bool bixenon = false;
    bool FSamLuc = false;
    bool FLEDZar = false;
    bool FLedFro = true;
    bool FLedRea = false;
    bool meglenke = false;
    bool headup = false;
    bool FVozPas = false;
    bool FSisMrt = false;
    bool EQroadsign = false;
    bool senzordez = true;
    bool AdLight = false;
    bool alarm = true;
    bool blokadamotorja = false;
    bool kodamotorja = false;
    bool FSamZav = true;
    bool FVozOpo = false;
    bool klima = true;
    bool aklima = true;
    bool klimazadaj = false;
    bool webasto = false;
    bool tonirana = false;
    bool estekla2 = true;
    bool estekla4 = false;
    bool eogledala = true;
    bool gretjeogledal = true;
    bool zloziogledala = false;
    bool centralno = true;
    bool daljinskocentralno = false;
    bool keyless = false;
    bool servo = true;
    bool usnjenvolan = true;
    bool multivolan = true;
    bool sportvolan = false;
    bool tempomat = true;
    bool tempomatplus = false;
    bool avtoradio = true;
    char avtoradioopis[BUFFER_SIZE] = "";
    bool USB = true;
    bool TV = false;
    bool racunalnik = true;
    bool komunikacijskipaket = false;
    bool navigacija = false;
    bool FTouScre = true;
    bool AndroidAuto = false;
    bool deljivaklop12 = false;
    bool deljivaklop13 = true;
    bool Isofix = false;
    bool otroskisedez = false;
    bool skibag = false;
    bool mreza = false;
    bool rolo = false;
    bool FSloNav = false;
    bool FDvoDno = false;
    bool sani = false;
    bool FSisKla = true;
    bool FSisPar = false;
    bool PDC = false;
    bool FParKam = false;
    bool FSenFro = false;
    bool FSenRea = false;
    bool FPar360 = false;
    bool FVzvKam = false;
    bool FBocSto = false;
    bool VK = false;
    bool ZaInvalide = false;
    char countdown[BUFFER_SIZE] = "4000";
    bool nazalogi = true;
    char opombe[BUFFER_SIZE] = "asdadasdadas";
    const char* streha_options[3] = {"trda", "mehka", "pomicna"};
    int selected_streha = 2;
    char vrata[BUFFER_SIZE] = "4";
    const char* oblazinjenje_options[3] = {"blago", "usnje", "alcantara"};
    int selected_oblazinjenje = 0;
    char barvaoblazin[BUFFER_SIZE] = "bela";
    bool fourwd = false;
    bool oldtimer = false;
    bool guarantee = false;
    bool jamstvo = false;
};

#endif //CAR_H
