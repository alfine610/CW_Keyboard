typedef struct _CWBranch CWBranch, *pCWBranch;
//cw data 2 branch
struct _CWBranch {
  //cw character
  char chara;
  //short branch
  pCWBranch nextShort;
  //long branch
  pCWBranch nextLong;
};

static CWBranch cw_None = { '\0', &cw_None, &cw_None };             //nothing signal loop

static CWBranch cw_backspace = { '\b', &cw_None, &cw_None };        //........
static CWBranch cw_ssssss = { '\0', &cw_backspace, &cw_None };      //.......

static CWBranch cw_calet = { '^', &cw_ssssss, &cw_None };           //......
static CWBranch cw_question = { '?', &cw_None, &cw_None };          //..--..
static CWBranch cw_underbar = { '_', &cw_None, &cw_None };          //..--.-
static CWBranch cw_doublequotation = { '"', &cw_None, &cw_None };   //.-..-.
static CWBranch cw_period = { '.', &cw_None, &cw_None };            //--..--
static CWBranch cw_at = { '@', &cw_None, &cw_None };                //.--.-.
static CWBranch cw_singlequotation = { '\'', &cw_None, &cw_None };  //.----.
static CWBranch cw_hyphen = { '-', &cw_None, &cw_None };            //-....-
static CWBranch cw_right = { ')', &cw_None, &cw_None };             //-.--.-
static CWBranch cw_comma = { ',', &cw_None, &cw_None };             //.-.-.-
static CWBranch cw_colon = { ':', &cw_None, &cw_None };             //---...

static CWBranch cw_5 = { '5', &cw_calet, &cw_None };                //.....
static CWBranch cw_4 = { '4', &cw_None, &cw_None };                 //....-
static CWBranch cw_3 = { '3', &cw_None, &cw_None };                 //...--
static CWBranch cw_sslls = { '\0', &cw_question, &cw_underbar };    //..--.
static CWBranch cw_2 = { '2', &cw_None, &cw_None };                 //..---
static CWBranch cw_slssl = { '\0', &cw_doublequotation, &cw_None }; //.-..-
static CWBranch cw_plus = { '+', &cw_None, &cw_period };            //.-.-.
static CWBranch cw_sllsl = { '\0', &cw_at, &cw_None };              //.--.-
static CWBranch cw_1 = { '1', &cw_singlequotation, &cw_None };      //.----
static CWBranch cw_6 = { '6', &cw_None, &cw_hyphen };               //-....
static CWBranch cw_slash = { '/', &cw_None, &cw_None };             //-..-.
static CWBranch cw_left = { '(', &cw_None, &cw_right };             //-.--.
static CWBranch cw_7 = { '7', &cw_None, &cw_None };                 //--...
static CWBranch cw_llssl = { '\0', &cw_None, &cw_comma };           //--..-
static CWBranch cw_8 = { '8', &cw_colon, &cw_None };                //---..
static CWBranch cw_9 = { '9', &cw_None, &cw_None };                 //----.
static CWBranch cw_0 = { '0', &cw_None, &cw_None };                 //-----

static CWBranch cw_H = { 'h', &cw_5, &cw_4 };                       //....
static CWBranch cw_V = { 'v', &cw_None, &cw_3 };                    //...-
static CWBranch cw_F = { 'f', &cw_None, &cw_None };                 //..-.
static CWBranch cw_ssll = { '\0', &cw_sslls, &cw_2 };               //..--
static CWBranch cw_L = { 'l', &cw_None, &cw_slssl };                //.-..
static CWBranch cw_slsl = { '\0', &cw_plus, &cw_None };             //.-.-
static CWBranch cw_P = { 'p', &cw_None, &cw_sllsl };                //.--.
static CWBranch cw_J = { 'j', &cw_None, &cw_1 };                    //.---
static CWBranch cw_B = { 'b', &cw_6, &cw_None };                    //-...
static CWBranch cw_X = { 'x', &cw_slash, &cw_None };                //-..-
static CWBranch cw_C = { 'c', &cw_None, &cw_None };                 //-.-.
static CWBranch cw_Y = { 'y', &cw_left, &cw_None };                 //-.--
static CWBranch cw_Z = { 'z', &cw_7, &cw_llssl };                   //--..
static CWBranch cw_Q = { 'q', &cw_None, &cw_None };                 //--.-
static CWBranch cw_llls = { '\0', &cw_8, &cw_None };                //---.
static CWBranch cw_llll = { '\0', &cw_9, &cw_0 };                   //----

static CWBranch cw_S = { 's', &cw_H, &cw_V };                       //...
static CWBranch cw_U = { 'u', &cw_F, &cw_ssll };                    //..-
static CWBranch cw_R = { 'r', &cw_L, &cw_slsl };                    //.-.
static CWBranch cw_W = { 'w', &cw_P, &cw_J };                       //.--
static CWBranch cw_D = { 'd', &cw_B, &cw_X };                       //-..
static CWBranch cw_K = { 'k', &cw_C, &cw_Y };                       //-.-
static CWBranch cw_G = { 'g', &cw_Z, &cw_Q };                       //--.
static CWBranch cw_O = { 'o', &cw_llls, &cw_llll };                 //---

static CWBranch cw_I = { 'i', &cw_S, &cw_U };                       //..
static CWBranch cw_A = { 'a', &cw_R, &cw_W };                       //.-
static CWBranch cw_N = { 'n', &cw_D, &cw_K };                       //-.
static CWBranch cw_M = { 'm', &cw_G, &cw_O };                       //--

static CWBranch cw_E = { 'e', &cw_I, &cw_A };                       //.
static CWBranch cw_T = { 't', &cw_N, &cw_M };                       //-

static CWBranch cw_Root = { '\0', &cw_E, &cw_T };                   //root (first use) 
static pCWBranch pcw_Root = &cw_Root;                               //root (first use)   