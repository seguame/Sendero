#include "manejador_class.h"

ManejadorClass* ManejadorClass::instancia = NULL;

ManejadorClass::ManejadorClass(){}

void ManejadorClass::Inicializar(void)
{
    Terminar();

    instancia = new ManejadorClass();
}

ManejadorClass* ManejadorClass::ObtenerInstancia(void)
{
    if(instancia == NULL)
    {
        instancia = new ManejadorClass();
    }

    return instancia;
}

void ManejadorClass::Terminar(void)
{
    if(instancia != NULL)
    {
        delete instancia;
        instancia = NULL;
    }
}

void ManejadorClass::resetearContadorLocalidades(void)
{
    localidadesActuales = 0;
}

void ManejadorClass::registrarVariableLocal(Simbolo* simbolo, int profundidad)
{
    stringstream id;
    int localidad;

    if(profundidad > 1)
    {
        id << simbolo->getIdentificador() << simbolo->getStringTipo() << profundidad;

        if(locales.find(id.str()) == locales.end())
        {
            simbolo->setAlias(id.str());


            localidad = localidadesActuales;
            //aumentar localidades del tamaño que requiera el dato
            switch(simbolo->getTipo())
            {
                case T_REAL:
                    //Los doubles toman 2 bytes del constant pool
                    localidadesActuales += 2;
                    break;
                default:
                    ++localidadesActuales;
                    break;
            }



            pair<int, Simbolo*> variable = make_pair<int, Simbolo*>(localidad, simbolo);
            pair<string, pair<int, Simbolo*> > insertable = make_pair(id.str(), variable);

            locales.insert(insertable);
        }
        //else variable ya registrada de otro nivel y con mismo tipo, simplemente se usa su ref
    }
    else
    {
        qDebug() << "Variable global intentando ser registrada como local";
    }
}

void ManejadorClass::deregistrarVariableLocal(Simbolo* simbolo)
{
    locales.erase(locales.find(simbolo->getAlias()));
}


void ManejadorClass::escribirCabeceraClase(void)
{
    aniadirInstruccion(".class public", nombreClase);
    aniadirInstruccion(".super", "java/lang/Object");

    //se añade una instancia estatica del Scanner de java
    aniadirInstruccion(".field static public", "lector Ljava/util/Scanner;");

    //Se añade un constructor default vacio
    aniadirInstruccion(".method public <init> :", "()V");
    aniadirInstruccion("    .limit stack", "1");
    aniadirInstruccion("    .limit locals", "1");
    aniadirInstruccion("    aload_0", "");
    aniadirInstruccion("    invokespecial", "java/lang/Object <init> ()V");
    aniadirInstruccion("    return", "");
    aniadirInstruccion(".end", "method");
}

void ManejadorClass::escribirConstructorEstatico(void)
{
    aniadirInstruccion(".method static <clinit> :", "()V");

    //se añade el constructor estatico para instancia el Scanner
    aniadirInstruccion("    .limit stack", "10");
    aniadirInstruccion("    .limit locals", "0");
    aniadirInstruccion("    new", "java/util/Scanner");
    aniadirInstruccion("    dup", "");
    aniadirInstruccion("    getstatic", "java/lang/System in Ljava/io/InputStream;");
    aniadirInstruccion("    invokespecial", "java/util/Scanner <init> (Ljava/io/InputStream;)V");
    aniadirInstruccion("    putstatic", nombreClase + " lector Ljava/util/Scanner;");

    //se inicializan los arrays y se ignoran globales sin dimension o constantes
    for(vector< Simbolo* >::size_type i = 0; i != globales.size(); ++i)
    {
        Simbolo* temp = globales.at(i);
        escribirDeclararArray(temp, true);
    }


    aniadirInstruccion("    return", "");
    aniadirInstruccion(".end", "method");
}

void ManejadorClass::escribirDeclararArray(Simbolo* temp, bool global)
{
    if(temp->getCantidadDimensiones() > 0)
    {
        unsigned int cantDimen = temp->getCantidadDimensiones();
        string operacion;
        stringstream operando;
        stringstream firmaDimensional;
        stringstream nombreCompleto;

        //ls locales ya fueron apilando sus valores
        if(global)
        {
            for(unsigned int i = 0; i < cantDimen; ++i)
            {
                //apilar las dimensiones
                escribirEnteroConstante(temp->getTamanioDimension(i));
            }
        }

        if(cantDimen == 1)
        {
            if(temp->getTipo() == T_CADENA) operacion = "    anewarray";
            else operacion = "    newarray";

            switch(temp->getTipo())
            {
                case T_ENTERO: operando << "int"; break;
                case T_REAL: operando << "double"; break;
                case T_CARACTER: operando << "char"; break;
                case T_BOOLEANO: operando << "boolean"; break;
                case T_CADENA: operando << "java/lang/String"; break;
                default: operando << "ERROR TIPO"; break;
            }

            firmaDimensional << "[" << obtenerTipo(temp->getTipo());
        }
        else
        {
            operacion = "    multianewarray";

            for(unsigned int i = 0; i < cantDimen; ++i)
            {
                firmaDimensional << "[";
            }

            firmaDimensional << obtenerTipo(temp->getTipo());

            operando << firmaDimensional.str() << " " << cantDimen;
        }

        aniadirInstruccion(operacion, operando.str());

        nombreCompleto << nombreClase << " " << temp->getIdentificador() << " " << firmaDimensional.str();

        if(global)
        {
            aniadirInstruccion("    putstatic", nombreCompleto.str());
        }
        else
        {
            string oper = "    astore";
            stringstream pos;
            pos << obtenerPosLocal(temp->getAlias());

            aniadirInstruccion(oper, pos.str());
        }
    }
}

void ManejadorClass::escribirMain(void)
{
    string metodo  = "main";
    string firma   = "9";

    escribirCabeceraMetodo(metodo, firma, T_INVALIDO);

    Simbolo* dummy = new Simbolo("Args");
    dummy->setTipo(T_CADENA)->setInicializado();
    registrarVariableLocal(dummy, 2);
}

void ManejadorClass::escribirCabeceraMetodo(Simbolo* funcion)
{
    if(funcion->getTipo() == T_FUNCION)
    {
        if(funcion->getIdentificador().compare("principal") == 0)
        {
            escribirMain();
        }
        else
        {
            escribirCabeceraMetodo(funcion->getIdentificador(), funcion->getFirmaFuncion(), funcion->getTipoRetorno());
        }
    }
    else
    {
        qDebug() << "Intentando escribir cabecera de metodo con un simbolo que no es funcion";
    }
}

void ManejadorClass::escribirCabeceraMetodo(const string& nombre, const string& firma, Tipo retorno)
{
    stringstream metodo;
    string firmaCompleta;

    metodo << ".method public static ";
    metodo << nombre;
    metodo << " :";

    firmaCompleta = obtenerDescriptorFirma(firma, retorno);

    aniadirInstruccion(metodo.str(), firmaCompleta);
    aniadirInstruccion("    .limit stack", "40");
    aniadirInstruccion("    .limit locals", "100");
}

void ManejadorClass::escribirFinMetodo(Simbolo* funcion)
{
    string retorno;

    retorno = obtenerTipoRetorno(funcion->getTipoRetorno());

    aniadirInstruccion(retorno, "");
    aniadirInstruccion(".end", "method");
}

void ManejadorClass::aniadirInstruccion(const string& operacion,const string& parametro)
{
    instrucciones.push_back(make_pair(operacion, parametro));
}

void ManejadorClass::escribirLeerDato(Simbolo* simbolo)
{
    string llamadaScanner = nombreClase + " lector Ljava/util/Scanner;";
    aniadirInstruccion("    getstatic", llamadaScanner);

    switch(simbolo->getTipo())
    {
        case T_ENTERO:
            aniadirInstruccion("    invokevirtual", "java/util/Scanner nextInt ()I");
            aniadirInstruccion("    getstatic", llamadaScanner);
            aniadirInstruccion("    invokevirtual", "java/util/Scanner nextLine ()Ljava/lang/String;"); //ignorar
            aniadirInstruccion("    pop", "");                                                          //saltos de linea
            break;

        case T_REAL:
            aniadirInstruccion("    invokevirtual", "java/util/Scanner nextDouble ()D");
            aniadirInstruccion("    getstatic", llamadaScanner);
            aniadirInstruccion("    invokevirtual", "java/util/Scanner nextLine ()Ljava/lang/String;"); //ignorar
            aniadirInstruccion("    pop", "");                                                          //saltos de linea
            break;

        case T_CADENA:
            aniadirInstruccion("    invokevirtual", "java/util/Scanner nextLine ()Ljava/lang/String;");
            break;

        case T_CARACTER:
            aniadirInstruccion("    invokevirtual", "java/util/Scanner next ()Ljava/lang/String;");
            aniadirInstruccion("    invokevirtual", "java/lang/String trim ()Ljava/lang/String;");
            aniadirInstruccion("    iconst_0", "");
            aniadirInstruccion("    invokevirtual", "java/lang/String charAt (I)C");
            break;

        default:
            //no se genera codigo de lectura para otra cosa
            break;
    }

    escribirLlamadaVariable(simbolo, true);
}

void ManejadorClass::prepararImpresionPantalla(void)
{
    aniadirInstruccion("    getstatic", "java/lang/System out Ljava/io/PrintStream;");
}

void ManejadorClass::escribirImpresionLNPantalla(const string& texto)
{
    aniadirInstruccion("    ldc", texto);
    aniadirInstruccion("    invokevirtual", "java/io/PrintStream println (Ljava/lang/Object;)V");
}

void ManejadorClass::escribirImpresionPantalla(const string& texto, Simbolo* s, Tipo t, bool esConstante)
{
    /*if(esConstante)
    {
        switch(t)
        {
        //case T_CADENA:
        case T_INVALIDO:
        case T_BOOLEANO:
            aniadirInstruccion("    ldc", texto);
            break;
        case T_ENTERO:
            escribirEnteroConstante(s->getValor<int>());
            break;
        case T_REAL:
            escribirRealConstante(s->getValor<double>());
            break;
        default: break;
        }
    }*/

    stringstream llamada;

    llamada << "java/io/PrintStream print ";

    switch(t)
    {
    case T_ENTERO:
        llamada << "(I)V";
        break;

    case T_REAL:
        llamada << "(D)V";
        break;

    case T_CARACTER:
        llamada << "(C)V";
        break;

    case T_BOOLEANO:
        llamada << "(Z)V";
        break;

    case T_FUNCION:
        switch(s->getTipoRetorno())
        {
            case T_ENTERO:
                llamada << "(I)V";
                break;

            case T_REAL:
                llamada << "(D)V";
                break;

            case T_CARACTER:
                llamada << "(C)V";
                break;

            case T_CADENA:
            case T_INVALIDO: //"NULL"
            case T_BOOLEANO: //"Verdadero" | "Falso"
                llamada << "(Ljava/lang/Object;)V";
                break;
        }
        break;

    case T_CADENA:
    case T_INVALIDO:
        llamada << "(Ljava/lang/Object;)V";
        break;
    }

    aniadirInstruccion("    invokevirtual", llamada.str());

}

void ManejadorClass::escribirDeclararVariableGlobal(Simbolo* simbolo)
{

    stringstream operando;
    unsigned int cantDimensiones;

    operando << simbolo->getIdentificador() << " ";

    cantDimensiones = simbolo->getCantidadDimensiones();



    for(unsigned int i = 0; i < cantDimensiones; ++i)
    {
        operando << "[";
    }

    operando << obtenerTipo(simbolo->getTipo());

    aniadirInstruccion(".field static public", operando.str());


    //añadir a nuestro compendio de llamables donde sea
    globales.push_back(simbolo);
}

void ManejadorClass::escribirDeclararConstante(Simbolo* simbolo)
{
    stringstream operando;
    operando << simbolo->getIdentificador();
    operando << " " << obtenerTipo(simbolo->getTipo());
    operando << " = ";
    switch(simbolo->getTipo())
    {
        case T_ENTERO:
            operando << simbolo->getValor<int>();
            break;
        case T_REAL:
            operando << simbolo->getValor<double>();
            break;
        case T_BOOLEANO:
            operando << simbolo->getValor<bool>();
            break;
        case T_CADENA:
            operando << (simbolo->getValor<string>());
            break;
        case T_CARACTER:
            operando << simbolo->getValor<char>();
            break;
        default:
            operando << "ERROR VALOR CONSTANTE";
            break;
    }

    globales.push_back(simbolo);
    aniadirInstruccion(".field static public final", operando.str());
}

void ManejadorClass::escribirSuma(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: aniadirInstruccion("    iadd", ""); break;
        case T_REAL: aniadirInstruccion("    dadd", ""); break;
        default: qDebug() << "Suma de tipos no soportada " << t;
    }
}

void ManejadorClass::escribirResta(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: aniadirInstruccion("    isub", ""); break;
        case T_REAL: aniadirInstruccion("    dsub", ""); break;
        default: qDebug() << "Resta de tipos no soportada " << t;
    }
}

void ManejadorClass::escribirMultiplicacion(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: aniadirInstruccion("    imul", ""); break;
        case T_REAL: aniadirInstruccion("    dmul", ""); break;
        default: qDebug() << "Multiplicacion de tipos no soportada " << t;
    }
}

void ManejadorClass::escribirDivision(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: aniadirInstruccion("    idiv", ""); break;
        case T_REAL: aniadirInstruccion("    ddiv", ""); break;
        default: qDebug() << "Division de tipos no soportada " << t;
    }
}

void ManejadorClass::escribirPotencia(void)
{
    aniadirInstruccion("    invokestatic", "java/lang/Math pow (DD)D");
}

void ManejadorClass::escribirModulo(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: aniadirInstruccion("    irem", ""); break;
        case T_REAL: aniadirInstruccion("    drem", ""); break;
        default: qDebug() << "Modulo (%) de tipos no soportada " << t;
    }
}

void ManejadorClass::escribirInvertirValor(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: aniadirInstruccion("    ineg", ""); break;
        case T_REAL: aniadirInstruccion("    dneg", ""); break;
        default: qDebug() << "Inversion de valor de tipos no soportada " << t;
    }
}

void ManejadorClass::escribirComparacion(string operando, string etiqueta)
{
    if(operando.compare(">") == 0)
        aniadirInstruccion("    if_icmpgt", etiqueta);
    else if(operando.compare("<") == 0)
        aniadirInstruccion("    if_icmplt", etiqueta);
    else if(operando.compare(">=") == 0)
        aniadirInstruccion("    if_icmpge", etiqueta);
    else if(operando.compare("<=") == 0)
        aniadirInstruccion("    if_icmple", etiqueta);
    else if(operando.compare("==") == 0)
        aniadirInstruccion("    if_icmpeq", etiqueta);
    else if(operando.compare("!=") == 0)
        aniadirInstruccion("    if_icmpne", etiqueta);
}

void ManejadorClass::escribirLlamadaVariable(Simbolo* simbolo, bool almacenar)
{
    bool esGlobal = false;

    int pos = obtenerPosLocal(simbolo->getAlias());

    if(pos == -1)
    {
        for(vector< Simbolo* >::size_type i = 0; i < globales.size(); ++i)
        {
            if(simbolo->getIdentificador().compare(globales.at(i)->getIdentificador()) == 0)
            {
                esGlobal = true;
                break;
            }
        }
    }

    if(!esGlobal)
    {
        if(simbolo->getCantidadDimensiones() > 0)
        {
            stringstream posStr;
            posStr << pos;

            if(almacenar)
                aniadirInstruccion("    astore", posStr.str());
            else
                aniadirInstruccion("    aload", posStr.str());
        }
        else
        {
            switch(simbolo->getTipo())
            {
                case T_ENTERO:
                case T_BOOLEANO: //se almacenan con 1 o 0
                case T_CARACTER: //internamente son enteros
                    escribirLlamadaVariableEntero(pos, almacenar);
                    break;

                case T_CADENA:
                    escribirLlamadaVariableAlfabetico(pos, almacenar);
                    break;

                case T_REAL:
                    escribirLlamadaVariableReal(pos, almacenar);
                    break;

                default:
                    qDebug() << "Llamada a una variable no definida";
                    break;
            }
        }
    }
    else
    {
        escribirLlamadaVarGlobal(simbolo, almacenar);
    }
}

void ManejadorClass::escribirValorConstante(Simbolo* simbolo)
{
    if(simbolo->getIdentificador().compare("HOLDER") != 0) qDebug() << "valor constante viene de una variable";

    switch(simbolo->getTipo())
    {
        case T_ENTERO:
            escribirEnteroConstante(simbolo->getValor<int>());
            break;

        case T_CARACTER:
            escribirEnteroConstante(simbolo->getValor<char>());
            break;

        case T_BOOLEANO:
            escribirEnteroConstante(simbolo->getValor<bool>());
            break;

        case T_REAL:
            escribirRealConstante(simbolo->getValor<double>());
            break;

        case T_CADENA:
            escribirStringConstante(simbolo->getValor<string>());
            break;

        default:
            qDebug() << "Valor no soportado en pila";
            break;
    }
}

void ManejadorClass::escribirLlamadaVariableEntero(int posStack, bool almacenar)
{
    stringstream operador;
    stringstream operando;

    if(almacenar)
    {
        if(posStack <= 3)
        {
            operador << "    istore_" << posStack;
            operando << "";
        }
        else
        {
            operador << "    istore";
            operando << posStack;
        }
    }
    else //cargar
    {
        if(posStack <= 3)
        {
            operador << "    iload_" << posStack;
            operando << "";
        }
        else
        {
            operador << "    iload";
            operando << posStack;
        }
    }

    aniadirInstruccion(operador.str(), operando.str());
}

void ManejadorClass::escribirLlamadaVariableReal(int posStack, bool almacenar)
{
    stringstream operador;
    stringstream operando;

    if(almacenar)
    {
        if(posStack <= 3)
        {
            operador << "    dstore_" << posStack;
            operando << "";
        }
        else
        {
            operador << "    dstore";
            operando << posStack;
        }
    }
    else //cargar
    {
        if(posStack <= 3)
        {
            operador << "    dload_" << posStack;
            operando << "";
        }
        else
        {
            operador << "    dload";
            operando << posStack;
        }
    }

    aniadirInstruccion(operador.str(), operando.str());
}


void ManejadorClass::escribirLlamadaVariableAlfabetico(int posStack, bool almacenar)
{
    stringstream operador;
    stringstream operando;

    if(almacenar)
    {
        if(posStack <= 3)
        {
            operador << "    astore_" << posStack;
            operando << "";
        }
        else
        {
            operador << "    astore";
            operando << posStack;
        }
    }
    else //cargar
    {
        if(posStack <= 3)
        {
            operador << "    aload_" << posStack;
            operando << "";
        }
        else
        {
            operador << "    aload";
            operando << posStack;
        }
    }

    aniadirInstruccion(operador.str(), operando.str());
}

void ManejadorClass::escribirLlamadaFuncion(Simbolo* simbolo)
{
    if(simbolo->getTipo() != T_FUNCION)
    {
        qDebug() << "Intenta llamar una variable como funcion en el bytecode";
    }
    else
    {
        stringstream llamada;

        llamada << nombreClase << " " << simbolo->getIdentificador() << " ";
        llamada << obtenerDescriptorFirma(simbolo->getFirmaFuncion(), simbolo->getTipoRetorno());

        aniadirInstruccion("    invokestatic", llamada.str());
    }
}

void ManejadorClass::escribirLlamadaVarGlobal(Simbolo* simbolo, bool almacenar)
{
    stringstream llamada;
    string operacion;

    if(almacenar)
        operacion = "    putstatic";
    else
        operacion = "    getstatic";

    llamada << nombreClase << " " << simbolo->getIdentificador() << " ";

    for(int i = 0; i < simbolo->getCantidadDimensiones(); ++i)
    {
        llamada << "[";
    }

    llamada << obtenerTipo(simbolo->getTipo());

    aniadirInstruccion(operacion, llamada.str());
}

void ManejadorClass::escribirEnteroConstante(int i)
{
    if(i == -1)
        aniadirInstruccion("    iconst_m1", "");
    else
    {
        stringstream operador;
        stringstream operando;

        operador << "    ";
        operando << "";

        if(i >= 0 && i <= 5)
        {
            operador << "iconst_";
            operador << i;
        }
        else if(i >= -128 && i <= 127)
        {
            operador << "bipush";
            operando << i;
        }
        else if(i >= -32768 && i <= 32767)
        {
            operador << "sipush";
            operando << i;
        }
        else
        {
            operador << "ldc_w";
            operando << i;
        }

        aniadirInstruccion(operador.str(), operando.str());
    }
}

void ManejadorClass::escribirRealConstante(double d)
{
    stringstream operador;
    stringstream operando;

    operador << "    ldc2_w";
    operando << d;

    aniadirInstruccion(operador.str(), operando.str());
}

void ManejadorClass::escribirStringConstante(const string& s)
{
    aniadirInstruccion("    ldc", s);
}

void ManejadorClass::escribirCastingDouble(void)
{
    aniadirInstruccion("    i2d", "");
}

void ManejadorClass::escribirArchivoParaEnsamblar(const string& ruta)
{
    archivo.open((ruta+".bytecode").c_str());

    if(!archivo.is_open())
    {
        qDebug() << "No se pudo abrir el archivo de bytecode";
        return;
    }

    for(vector<pair<string,string> >::size_type i = 0; i != instrucciones.size(); ++i)
    {
        archivo << instrucciones[i].first;

        if(instrucciones[i].second.compare("") != 0)
        {
            archivo << " " << instrucciones[i].second;
        }

        archivo << endl;
    }

    qDebug() << "Archivo bytecode escrito";
}

int ManejadorClass::Ensamblar(const string& ruta, const string& archivo)
{
    stringstream comando;
    stringstream comando2;

    comando << "python ";
    comando << "\"/home/seguame/Documentos/Taller Compiladores/EditorSendero/compilador/Krakatau/assemble.py\" \"";
    comando << ruta;
    comando << "/";
    comando << archivo;
    comando << ".bytecode\"";

    qDebug() << comando.str().c_str();

    system(comando.str().c_str());

    comando2 << "mv ";
    comando2 << "\"/home/seguame/Documentos/Taller Compiladores/EditorSendero/compilador/Krakatau/";
    comando2 << archivo;
    comando2 << ".class\" \"";
    comando2 << ruta;
    comando2 << "/";
    comando2 << archivo;
    comando2 << ".class\"";

    qDebug() << comando2.str().c_str();

    return system(comando2.str().c_str());


    //return system(("java " + archivo).c_str());
}

void ManejadorClass::setNombreClase(const string& nombre)
{
    nombreClase = nombre;
}

string ManejadorClass::obtenerDescriptorFirma(const string& firma, Tipo retorno)
{
    stringstream flujo;
    int longitud = firma.length();

    flujo << '(';

    for(int i = 0; i < longitud; ++i)
    {
        flujo << obtenerTipo(firma[i]);
    }

    flujo << ')';

    flujo << obtenerTipo(retorno);

    return flujo.str();
}

string ManejadorClass::obtenerTipoRetorno(Tipo retorno)
{
    switch(retorno)
    {
        case T_CARACTER:
        case T_ENTERO:
            return "    ireturn";
        case T_REAL:
            return "    dreturn";
        default:
            return "    return";
    }
}

string ManejadorClass::obtenerTipo(char t)
{
    switch(t)
    {
        case '0': return "I";
        case '1': return "D";
        case '2': return "C";
        case '3': return "Ljava/lang/String;";
        case '4': return "Z";
        case '6': return ""; //no se especifica para parametros
        case '9': return "[Ljava/lang/String;";
        default:  return "ERROR_SIMBOLO_NO_ADMITIDO";
    }
}

string ManejadorClass::obtenerTipo(Tipo t)
{
    switch(t)
    {
        case T_ENTERO: return "I";
        case T_REAL: return "D";
        case T_CARACTER: return "C";
        case T_CADENA: return "Ljava/lang/String;";
        case T_BOOLEANO: return "Z";
        case T_INVALIDO: return "V";
        default: return "ERROR_SIMBOLO_NO_ADMITIDO";
    }
}

int ManejadorClass::obtenerPosLocal(const string& alias)
{
    if(locales.find(alias) != locales.end())
        return locales[alias].first;
    else
        return -1;
}
