#ifndef BDINCH
#define BDINCH

//---------------------------------------------------------------------------
// Constantes
#define IDINDEFINIDO -1      // identificador inv�lido ou desconhecido

//-----------------------------------------------------------------------------
// Macros para arquivo de log
//-----------------------------------------------------------------------------
#ifndef SNPLOGFILENAME
#define SNPLOGFILENAME "SinapDBErr.log"
#endif

//#define SNPSTRLINEID(x)    x.sprintf("%s (%s) => %08d", __FUNC__, ExtractFileName(__FILE__), __LINE__)
#define SNPSTRLINEID(x)   (x.sprintf("%s (%s) => %08d", WideString(__FUNC__).c_bstr(), WideString(ExtractFileName(__FILE__)).c_bstr(), __LINE__))
#define SNPSTRLOG(x)       if (plog) { plog->Add(x); }
#define SNPSTRLOGLINE(x)   SNPSTRLINEID(x); SNPSTRLOG(x);
#define SNPSTRLOGINFO      "Verifique mensagem gravada no arquivo: %s"

// mml@20101013
// Essa estrutura armazena informa��es adicionais para as curvas
// Nem sempre todas as informa��es s�o v�lidas
struct strCURHEADER
   {
   unsigned char uchTipoEquipId;  // Tipo de equipamento associado � curva
   unsigned char uchClasseId;     // Classe de consumidor - curva de carga para consumidores
   float         fltValorMin;     // Utilizado para consumo, por exemplo
   float         fltValorMax;     // Utilizado para consumo, por exemplo
   short         shtUnidade;      // Somente v�lido para unidade de fluxo de pot�ncia
   short         shtEscala;       // ver patESCALA em const.h
   unsigned char uchNumValores;   // Numero de valores por patamar. Para todos os patamares � o mesmo n�mero
   };

// Par�metros do driver Oracle para dimensionamento dos buffers
// Original � 25. Quanto maior, menos vezes tem que iterar para buscar dados
#define ORACLE_DRIVER_BUFFER_SIZE 100

//---------------------------------------------------------------------------
// Mensagens de erro
// MSGERRSYS_  : Mensagens de erro referentes a problemas internos (ponteiros inv�lidos, por exemplo)
// MSGERRBDRD_ : Mensagens de erro referentes a problemas na leitura da base de dados
// MSGERRBDWR_ : Mensagens de erro referentes a problemas na escrita na base de dados
//
#define MSGERRSYS_ACAO                        Erro("A��o inv�lida.");
#define MSGERRSYS_ACAOINV                     Erro("A��o inv�lida.");
#define MSGERRSYS_ALTERNATIVA                 Erro("Alternativa inv�lida.");
#define MSGERRSYS_ALTERNATIVAINV              Erro("Alternativa inv�lida.");
#define MSGERRSYS_AREAINV                     Erro("�rea inv�lida.");
#define MSGERRSYS_ARRANJOINV                  Erro("Arranjo inv�lido.");
#define MSGERRSYS_ARRANJOSINV                 Erro("Arranjos inv�lidos.");
#define MSGERRSYS_ARSESPINV                   Erro("ARSESP inv�lida.");
#define MSGERRSYS_BALANCOINV                  Erro("Balan�o inv�lido.");
#define MSGERRSYS_BARRAINV                    Erro("Barra inv�lida.");
#define MSGERRSYS_BATERIAINV                  Erro("Bateria inv�lida.");
#define MSGERRSYS_BDFUNCINV                   Erro("M�dulo de fun��es de bancos de dados inv�lido.");
#define MSGERRSYS_BDGERALINV                  Erro("Banco de dados inv�lido.");
#define MSGERRSYS_BDCURVAINV                  Erro("Banco de dados Curva inv�lido.");
#define MSGERRSYS_CABOINV                     Erro("Cabo inv�lido.");
#define MSGERRSYS_CANALINV                    Erro("Canal inv�lido.");
#define MSGERRSYS_CAPACITORINV                Erro("Capacitor inv�lido.");
#define MSGERRSYS_CAPSERIEINV                 Erro("Capacitor s�rie inv�lido.");
#define MSGERRSYS_CARGAINV                    Erro("Carga inv�lida.");
#define MSGERRSYS_CENARIOINV                  Erro("Cen�rio inv�lido.");
#define MSGERRSYS_CHAVEINV                    Erro("Chave inv�lida.");
#define MSGERRSYS_CHAVETIPOINV                Erro("Tipo de chave inv�lido para a chave " + chave->Codigo);
#define MSGERRSYS_CLASSESINV                  Erro("Classes de consumidores inv�lidas.");
#define MSGERRSYS_CLUSTERINV                  Erro("Cluster inv�lido.");
#define MSGERRSYS_CORTEINV                    Erro("Corte inv�lida.");
#define MSGERRSYS_CRESCIMENTOINV              Erro("Crescimento inv�lido.");
#define MSGERRSYS_CURVAINV                    Erro("Curva inv�lida.");
#define MSGERRSYS_CURVASTREAM                 Erro("Erro na montagem da curva para o equipamento " + eqpto->TipoAsString() + " - " + eqpto->Codigo);
#define MSGERRSYS_CURVASTREAM2                Erro("Erro na montagem da curva");
#define MSGERRSYS_DOMINIOINV                  Erro("Dom�nio inv�lido.");
#define MSGERRSYS_EMPRESAINV                  Erro("Empresa inv�lida.");
#define MSGERRSYS_EQUIPINV                    Erro("Equipamento inv�lido.");
#define MSGERRSYS_ESTUDOINV                   Erro("Estudo inv�lido.");
#define MSGERRSYS_FILTROINV                   Erro("Filtro inv�lido.");
#define MSGERRSYS_GERADORINV                  Erro("Gerador inv�lido.");
#define MSGERRSYS_GERALINV                    Erro("Configura��o geral inv�lida.");
#define MSGERRSYS_ITEMSOBRA                   Erro("ItemsObra inv�lidos.");
#define MSGERRSYS_ITEMOBRA                    Erro("ItemObra inv�lido.");
#define MSGERRSYS_LISARRANJOS                 Erro("Lista de arranjos inv�lida.");
#define MSGERRSYS_LISCABO                     Erro("Lista de cabos inv�lida.");
#define MSGERRSYS_LISCORTEINV                 Erro("Lista de cortes inv�lida.");
#define MSGERRSYS_LISDOMINIOINV               Erro("Lista de dom�nios inv�lida.");
#define MSGERRSYS_LISMREDEINV                 Erro("Lista de MRedes inv�lida.");
#define MSGERRSYS_LISOBJINV                   Erro("Lista inv�lida.");
#define MSGERRSYS_LISREDUINV                  Erro("Lista de redu��es inv�lida.");
#define MSGERRSYS_LISSUPORTE                  Erro("Lista de suportes inv�lida.");
#define MSGERRSYS_MANOBRAINV                  Erro("Manobra inv�lida.");
#define MSGERRSYS_MANOBRASINV                 Erro("Manobras inv�lido.");
#define MSGERRSYS_MATCSTREAM                  Erro("Erro na montagem da matriz de capacit�ncias.");
#define MSGERRSYS_MATYINV                     Erro("Matriz de admit�ncias inv�lida.");
#define MSGERRSYS_MATZSTREAM                  Erro("Erro na montagem da matriz de imped�ncias.");
#define MSGERRSYS_MEDIDORINV                  Erro("Medidor inv�lido.");
#define MSGERRSYS_MERCADOINV                  Erro("Mercado inv�lido.");
#define MSGERRSYS_MONTAINV                    Erro("Montador inv�lido.");
#define MSGERRSYS_MREDEINV                    Erro("MRede inv�lida.");
#define MSGERRSYS_MUTUABARINV                 Erro("Barra(s) inv�lido(s) para a m�tua " + mutua->Codigo);
#define MSGERRSYS_MUTUAINV                    Erro("M�tua inv�lida.");
#define MSGERRSYS_MUTUATREINV                 Erro("Trecho(s) inv�lido(s) para a m�tua " + mutua->Codigo);
#define MSGERRSYS_OBRAINV                     Erro("Obra inv�lida.");
#define MSGERRSYS_OBRAS                       Erro("Obras inv�lidas.");
#define MSGERRSYS_PATHINV                     Erro("Diret�rios inv�lidos.");
#define MSGERRSYS_PATAMARESINV                Erro("Patamares inv�lidos.");
#define MSGERRSYS_PERDAINV                    Erro("Perda inv�lida.");
#define MSGERRSYS_PONTINV                     Erro("Ponteiro inv�lido.");
#define MSGERRSYS_PTOFIX                      Erro("Ponto de fixa��o inv�lido para o suporte " + suporte->Codigo + ".");
#define MSGERRSYS_RADIALINV                   Erro("Radial inv�lida.");
#define MSGERRSYS_REATORINV                   Erro("Reator inv�lido.");
#define MSGERRSYS_REDEINV                     Erro("Rede inv�lida.");
#define MSGERRSYS_REDESINV                    Erro("Redes inv�lidas.");
#define MSGERRSYS_REDUCAOINV                  Erro("Redu��o inv�lida.");
#define MSGERRSYS_SEGMENTOINV                 Erro("Segmento inv�lida.");
#define MSGERRSYS_REGULADORINV                Erro("Regulador inv�lido.");
#define MSGERRSYS_REGIAOINV                   Erro("Regi�o inv�lida.");
#define MSGERRSYS_SESSION                     Erro("Sess�o inv�lida.");
#define MSGERRSYS_SESSIONCON                  Erro("Sess�o v�lida, por�m n�o h� conex�o com a base de dados.");
#define MSGERRSYS_SINCRONISMOINV              Erro("Sincronismo inv�lido.");
#define MSGERRSYS_SINCRONISMOSINV             Erro("Sincronismos inv�lido.");
#define MSGERRSYS_STREAM                      Erro("Erro na montagem dos pontos de uma curva.");
#define MSGERRSYS_SUPORTEINV                  Erro("Suporte inv�lido.");
#define MSGERRSYS_SUPRIMENTOINV               Erro("Suprimento inv�lido.");
#define MSGERRSYS_TIPOREDEINV                 Erro("Tipo de rede inv�lido.");
#define MSGERRSYS_TIPOSINV                    Erro("Tipos inv�lidos.");
#define MSGERRSYS_TRAFO3EINV                  Erro("Trafo 3E inv�lido.");
#define MSGERRSYS_TRAFOINV                    Erro("Trafo inv�lido.");
#define MSGERRSYS_TRAFOMATY                   Erro("Erro na montagem da matriz de admit�ncias para o trafo " + trafo->Codigo);
#define MSGERRSYS_TRAFOZZINV                  Erro("TrafoZZ inv�lido.");
#define MSGERRSYS_TRATAFIMDADOS               Erro("Erro no tratamento final dos dados lidos.");
#define MSGERRSYS_TRECHOINV                   Erro("Trecho inv�lido.");
#define MSGERRSYS_YREFINV                     Erro("Matriz de admit�ncias inv�lida.");

#define MSGERRBDDL_CORTES                     Erro("Erro na remo��o dos cortes.");
#define MSGERRBDDL_DOMREDE                    Erro("Erro na remo��o dos dom�nios da rede " + rede->Codigo);
#define MSGERRBDDL_ESTUDOALT                  Erro("Erro na remo��o das alternativas do estudo " + estudo->Codigo);
#define MSGERRBDDL_REGIAO                     Erro("Erro na remo��o das regi�es");
#define MSGERRBDDL_SEGMENTO                   Erro("Erro na remo��o do segmento " + segmento->Codigo);
#define MSGERRBDDL_TRAFOENROL                 Erro("Erro na remo��o dos enrolamentos de uma trafo");
#define MSGERRBDRD_ARRANJO                    Erro("Erro na leitura do arranjo");
#define MSGERRBDRD_CABO                       Erro("Erro na leitura do cabo " + str_cabo.codigo);
#define MSGERRBDRD_CLASSE                     Erro("Erro na leitura de uma classe.");
#define MSGERRBDRD_CURTIPICAS                 Erro("Erro na leitura das curvas t�picas.");
#define MSGERRBDRD_CURCRESCIMENTO             Erro("Erro na leitura da curva de crescimento.");
#define MSGERRBDRD_DOMINIO                    Erro("Erro na leitura dos dom�nios.");
#define MSGERRBDRD_ESTUDO                     Erro("Erro na leitura do estudo " + estudo->Codigo);
#define MSGERRBDRD_ESTUDOALTERNATIVA          Erro("Erro na leitura das alternativas do estudo " + estudo->Codigo);
#define MSGERRBDRD_GERAL                      Erro("Erro na leitura das configura��es gerais");
#define MSGERRBDRD_MATCARRANJO                Erro("Erro na leitura da matriz C do arranjo " + str_arranjo2.codigo);
#define MSGERRBDRD_MATZARRANJO                Erro("Erro na leitura da matriz Z do arranjo " + str_arranjo2.codigo);
#define MSGERRBDRD_MERCADO                    Erro("Erro na leitura do mercado.");
#define MSGERRBDRD_NUMPONTOSFIX               Erro("Erro no n�mero de pontos de fixa��o");
#define MSGERRBDRD_POLIGONO                   Erro("Erro na leitura de um pol�gono.");
#define MSGERRBDRD_REDE                       Erro("Erro na leitura das redes do estudo " + estudo->Codigo);
#define MSGERRBDRD_REDEMREDE                  Erro("Erro na leitura das redes que comp�em a MRede " + mrede->Codigo);
#define MSGERRBDRD_RRBARRA                    Erro("Erro na leitura das barras reduzidas.");
#define MSGERRBDRD_RRCARGA                    Erro("Erro na leitura das cargas reduzidas.");
#define MSGERRBDRD_RRESTUDO                   Erro("Erro na leitura das redu��es do estudo " + estudo->Codigo);
#define MSGERRBDRD_RRSUPRIMENTO               Erro("Erro na leitura dos suprimentos reduzidos.");
#define MSGERRBDRD_RRTRAFO                    Erro("Erro na leitura dos transformadores reduzidos.");
#define MSGERRBDRD_RRYREF                     Erro("Erro na leitura das matrizes de admit�ncia reduzidas.");
#define MSGERRBDRD_SUPORTE                    Erro("Erro na leitura do suporte " + str_suporte.codigo);
#define MSGERRBDRD_TIPOCHAVE                  Erro("Erro na leitura dos tipos de chaves.");
#define MSGERRBDRD_TIPOCONSUMIDOR             Erro("Erro na leitura das classes de consumidores.");
#define MSGERRBDRD_TIPOREDE                   Erro("Erro na leitura dos tipos de redes.");
#define MSGERRBDRD_TIPOREDE2                  Erro("Erro na leitura do tipo de rede " + str_tipo_rede.codigo);

#define MSGERRBDWR_ARRANJO                    Erro("Erro na escrita do arranjo " + trecho->arranjo->Codigo);
#define MSGERRBDWR_ARRANJO2                   Erro("Erro na escrita do arranjo " + arranjo->Codigo);
#define MSGERRBDWR_ARRANJOS                   Erro("Erro na escrita dos arranjos.");
#define MSGERRBDWR_CLUSTERS                   Erro("Erro na escrita da lista de agrupamentos (clusters).");
#define MSGERRBDWR_CORTES                     Erro("Erro na escrita dos cortes.");
#define MSGERRBDWR_CURVAS                     Erro("Erro na escrita das curvas.");
#define MSGERRBDWR_CURCRESCIMENTO             Erro("Erro na escrita da curva de crescimento.");
#define MSGERRBDWR_CURCRESCIMENTO             Erro("Erro na escrita da curva de crescimento.");
#define MSGERRBDWR_DLEQUIPREDE(straux, streq) straux.sprintf("Erro na remo��o. Tipo de equipamento: %s, rede: %s, estudo: %d.", streq, rede->Codigo.c_str(), estudo->Id);
#define MSGERRBDWR_DOMINIOS                   Erro("Erro na escrita dos dom�nios.");
#define MSGERRBDWR_DOMREDE                    Erro("Erro na associa��o de um dom�nio � rede " + rede->Codigo);
#define MSGERRBDWR_EQPTO                      Erro("Erro na escrita do equipamento");
#define MSGERRBDWR_ESTUDO                     Erro("Erro na escrita do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOACAO                 Erro("Erro na escrita das a��es do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOALT                  Erro("Erro na escrita das alternativas do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOEQUIP                Erro("Erro na escrita dos equipamentos do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOEQUIP                Erro("Erro na escrita dos equipamentos do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOOBRA                 Erro("Erro na escrita das obras do estudo " + estudo->Codigo);
#define MSGERRBDWR_LIXEIRA                    Erro("Erro na remo��o de itens eliminados no estudo " + estudo->Codigo);
#define MSGERRBDWR_POLIGONO                   Erro("Erro na escrita de um pol�gono");
#define MSGERRBDWR_REDES                      Erro("Erro na escrita das redes do estudo " + estudo->Codigo);
#define MSGERRBDWR_SEGMENTO                   Erro("Erro na grava��o da regi�o " + segmento->Codigo);
#define MSGERRBDWR_SEGMENTO_CRESCIMENTO       Erro("Erro na grava��o do crescimento " + crescimento->Codigo + " da regi�o " + segmento->Codigo);
#define MSGERRBDWR_RRCARGA                    Erro("Erro na escrita das cargas reduzidas.");
#define MSGERRBDWR_RRSUPRIMENTO               Erro("Erro na escrita dos suprimentos reduzidos.");
#define MSGERRBDWR_RRTRAFO                    Erro("Erro na escrita dos transformadores reduzidos.");
#define MSGERRBDWR_RRYREF                     Erro("Erro na escrita das matrizes de admit�ncia reduzidas.");
#define MSGERRBDWR_SUPORTE                    Erro("Erro na escrita do suporte " + suporte->Codigo);
#define MSGERRBDWR_SUPORTEUTILIZADO           Erro("Erro na escrita do suporte " + suporte->Codigo + ".\nEsse suporte est� sendo utilizado.");
#define MSGERRBDWR_WREQUIPREDE(straux, streq) straux.sprintf("Erro na escrita. Tipo de equipamento: %s, estudo: %d.", streq, estudo->Id);

#define MSGERRBD_PARAMINV                     Erro("Par�metro inv�lido.");
#define MSGERRBD_EXECINV                      Erro("Erro na execu��o.");

#endif
