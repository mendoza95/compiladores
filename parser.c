#include <set>

#include "lista.h"
#include "parser.h"

using namespace std;

token t;
int e = 0;

/***********conjuntos primero*******/

set<int> p_jsonml({L_CORCHETE,LITERAL_CADENA});
set<int> p_element({L_CORCHETE,LITERAL_CADENA});
set<int> p_tname({LITERAL_CADENA});
set<int> p_atts({L_LLAVE});
set<int> p_att_list({LITERAL_CADENA});
set<int> p_att_list1({COMA,EPSILON});
set<int> p_att({LITERAL_CADENA});
set<int> p_att_name({LITERAL_CADENA});
set<int> p_att_value({LITERAL_CADENA,LITERAL_NUM,PR_TRUE,PR_FALSE,PR_NULL});
set<int> p_ele_list({L_CORCHETE,LITERAL_CADENA});
set<int> p_ele_list1({COMA,EPSILON});

/*************conjuntos siguiente*********/


set<int> s_jsonml({EOF});
set<int> s_element({EOF,COMA,R_CORCHETE});
set<int> s_tname({COMA,R_CORCHETE});
set<int> s_atts({COMA,R_CORCHETE});
set<int> s_att_list({COMA,R_LLAVE});
set<int> s_att_list1({COMA,R_LLAVE});
set<int> s_att({COMA,R_LLAVE});
set<int> s_att_name({DOS_PUNTOS});
set<int> s_att_value({COMA,R_LLAVE});
set<int> s_ele_list({R_CORCHETE,COMA,EOF});
set<int> s_ele_list1({R_CORCHETE,COMA,EOF});

set<int>::iterator i;

void iterar(set<int> cont){
    i = cont.begin();
    while(i != cont.end()) printf("%d\t",*i++);
}

void check_imput(set<int> firsts, set<int> follows){
    
    if (!(firsts.count(t.complexico))){
        error();
        scanto(unionSet(firsts,follows));
    }
}

void scanto(set<int> synchset){
    synchset.insert(EOF);
    while(!(synchset.count(t.complexico)))t  = extraer();
}

void error(){
    printf("Error sintactico en linea: %d. No se esperaba %s, comlexico: %d\n",t.numlinea,t.lexema,t.complexico);
    e ++;
}

void match(int lex){
    
    if (t.complexico == lex){
        //printf("matchea %s linea %d\n",t.lexema,t.numlinea);
        if (!esta_vacia()) t = extraer();
    }
    else{
        error();
    }
}

void jsonml(){
    t = extraer();
    check_imput(p_jsonml, s_jsonml);
    element(s_element);
    match(EOF);
    if (e == 0) printf("No se encontraron errores en sintacticos\n");
}

void element(set<int> synchset){
    check_imput(p_element,synchset);
    if (t.complexico == L_CORCHETE){
        
        match(L_CORCHETE);
        tag_name(s_tname);
        if(t.complexico == COMA){
            match(COMA);
            if (t.complexico == L_LLAVE){
                attributes(s_atts);
                if(t.complexico == COMA){
                    match(COMA);
                    element_list(s_ele_list);
                    match(R_CORCHETE);
                }
                else{
                    match(R_CORCHETE);
                }
            }
            else{
                element_list(s_ele_list);
                match(R_CORCHETE);
            }
        }
        else if (t.complexico == R_CORCHETE){
            match(R_CORCHETE);
        }
    }
    else if(t.complexico == LITERAL_CADENA){
        match(LITERAL_CADENA);
    }
    check_imput(synchset,p_element);
}

void tag_name(set<int> synchset){
    check_imput(p_tname,synchset);    
    match(LITERAL_CADENA);
    check_imput(synchset,p_tname);
}

void attributes(set<int> synchset){
    check_imput(p_atts,synchset);
    if (t.complexico == L_LLAVE){
        match(L_LLAVE);
        attribute_list(s_att_list);
        match(R_LLAVE);
    }
    else{
        match(L_LLAVE);
        match(R_LLAVE);
    }
    check_imput(synchset,p_atts);
}

void attribute_list(set<int> synchset){
    check_imput(p_att_list,synchset);
    attribute(s_att);
    attribute_list_1(s_att_list1);
    check_imput(synchset,p_att_list);
}

void attribute_list_1(set<int> synchset){
    if(t.complexico == COMA){
        check_imput(p_att_list1,synchset);
        match(COMA);
        attribute(s_atts);
        attribute_list_1(s_att_list1);
    }
    check_imput(synchset,p_att_list1);
}

void attribute(set<int> synchset){
    check_imput(p_att,synchset);
    attribute_name(s_att_name);
    match(DOS_PUNTOS);
    attribute_value(s_att_value);
    check_imput(synchset,p_att);
}

void attribute_name(set<int> synchset){
    check_imput(p_att_name, synchset);
    
    match(LITERAL_CADENA);
    check_imput(synchset,p_att_name);
}

void attribute_value(set<int> synchset){
    check_imput(p_att_value, synchset);
    
    switch (t.complexico){
        case LITERAL_CADENA:
            match(LITERAL_CADENA);
            break;
        case LITERAL_NUM:
            match(LITERAL_NUM);
            break;
        case PR_TRUE:
            match(PR_TRUE);
            break;
        case PR_FALSE:
            match(PR_FALSE);
            break;
        case PR_NULL:
            match(PR_NULL);
            break;
    }
    check_imput(synchset,p_att_value);
}

void element_list(set<int> synchset){
    check_imput(p_ele_list,synchset);
    
    element(s_element);
    element_list_1(s_ele_list1);
    check_imput(synchset,p_ele_list);
}

void element_list_1(set<int> synchset){
    if (t.complexico == COMA){
        check_imput(p_ele_list1,synchset);
        match(COMA);
        element(s_element);
        element_list_1(s_ele_list1);
    }
    check_imput(synchset,p_ele_list1);
}

set<int> unionSet(set<int> firsts,set<int> follow){
    set<int> nuevo;
    for (auto it1 = firsts.begin() ; it1!= firsts.end() ; it1 ++)
        nuevo.insert(*it1);

    for (auto it = follow.begin() ; it != follow.end() ; it ++)
        nuevo.insert(*it);
    return nuevo;
}
