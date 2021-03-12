#ifndef BDINCH
#define BDINCH

//---------------------------------------------------------------------------
// Constantes
#define IDINDEFINIDO -1      // identificador inválido ou desconhecido

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
// Essa estrutura armazena informações adicionais para as curvas
// Nem sempre todas as informações são válidas
struct strCURHEADER
   {
   unsigned char uchTipoEquipId;  // Tipo de equipamento associado à curva
   unsigned char uchClasseId;     // Classe de consumidor - curva de carga para consumidores
   float         fltValorMin;     // Utilizado para consumo, por exemplo
   float         fltValorMax;     // Utilizado para consumo, por exemplo
   short         shtUnidade;      // Somente válido para unidade de fluxo de potência
   short         shtEscala;       // ver patESCALA em const.h
   unsigned char uchNumValores;   // Numero de valores por patamar. Para todos os patamares é o mesmo número
   };

// Parâmetros do driver Oracle para dimensionamento dos buffers
// Original é 25. Quanto maior, menos vezes tem que iterar para buscar dados
#define ORACLE_DRIVER_BUFFER_SIZE 100

//---------------------------------------------------------------------------
// Mensagens de erro
// MSGERRSYS_  : Mensagens de erro referentes a problemas internos (ponteiros inválidos, por exemplo)
// MSGERRBDRD_ : Mensagens de erro referentes a problemas na leitura da base de dados
// MSGERRBDWR_ : Mensagens de erro referentes a problemas na escrita na base de dados
//
#define MSGERRSYS_ACAO                        Erro("Ação inválida.");
#define MSGERRSYS_ACAOINV                     Erro("Ação inválida.");
#define MSGERRSYS_ALTERNATIVA                 Erro("Alternativa inválida.");
#define MSGERRSYS_ALTERNATIVAINV              Erro("Alternativa inválida.");
#define MSGERRSYS_AREAINV                     Erro("Área inválida.");
#define MSGERRSYS_ARRANJOINV                  Erro("Arranjo inválido.");
#define MSGERRSYS_ARRANJOSINV                 Erro("Arranjos inválidos.");
#define MSGERRSYS_ARSESPINV                   Erro("ARSESP inválida.");
#define MSGERRSYS_BALANCOINV                  Erro("Balanço inválido.");
#define MSGERRSYS_BARRAINV                    Erro("Barra inválida.");
#define MSGERRSYS_BATERIAINV                  Erro("Bateria inválida.");
#define MSGERRSYS_BDFUNCINV                   Erro("Módulo de funções de bancos de dados inválido.");
#define MSGERRSYS_BDGERALINV                  Erro("Banco de dados inválido.");
#define MSGERRSYS_BDCURVAINV                  Erro("Banco de dados Curva inválido.");
#define MSGERRSYS_CABOINV                     Erro("Cabo inválido.");
#define MSGERRSYS_CANALINV                    Erro("Canal inválido.");
#define MSGERRSYS_CAPACITORINV                Erro("Capacitor inválido.");
#define MSGERRSYS_CAPSERIEINV                 Erro("Capacitor série inválido.");
#define MSGERRSYS_CARGAINV                    Erro("Carga inválida.");
#define MSGERRSYS_CENARIOINV                  Erro("Cenário inválido.");
#define MSGERRSYS_CHAVEINV                    Erro("Chave inválida.");
#define MSGERRSYS_CHAVETIPOINV                Erro("Tipo de chave inválido para a chave " + chave->Codigo);
#define MSGERRSYS_CLASSESINV                  Erro("Classes de consumidores inválidas.");
#define MSGERRSYS_CLUSTERINV                  Erro("Cluster inválido.");
#define MSGERRSYS_CORTEINV                    Erro("Corte inválida.");
#define MSGERRSYS_CRESCIMENTOINV              Erro("Crescimento inválido.");
#define MSGERRSYS_CURVAINV                    Erro("Curva inválida.");
#define MSGERRSYS_CURVASTREAM                 Erro("Erro na montagem da curva para o equipamento " + eqpto->TipoAsString() + " - " + eqpto->Codigo);
#define MSGERRSYS_CURVASTREAM2                Erro("Erro na montagem da curva");
#define MSGERRSYS_DOMINIOINV                  Erro("Domínio inválido.");
#define MSGERRSYS_EMPRESAINV                  Erro("Empresa inválida.");
#define MSGERRSYS_EQUIPINV                    Erro("Equipamento inválido.");
#define MSGERRSYS_ESTUDOINV                   Erro("Estudo inválido.");
#define MSGERRSYS_FILTROINV                   Erro("Filtro inválido.");
#define MSGERRSYS_GERADORINV                  Erro("Gerador inválido.");
#define MSGERRSYS_GERALINV                    Erro("Configuração geral inválida.");
#define MSGERRSYS_ITEMSOBRA                   Erro("ItemsObra inválidos.");
#define MSGERRSYS_ITEMOBRA                    Erro("ItemObra inválido.");
#define MSGERRSYS_LISARRANJOS                 Erro("Lista de arranjos inválida.");
#define MSGERRSYS_LISCABO                     Erro("Lista de cabos inválida.");
#define MSGERRSYS_LISCORTEINV                 Erro("Lista de cortes inválida.");
#define MSGERRSYS_LISDOMINIOINV               Erro("Lista de domínios inválida.");
#define MSGERRSYS_LISMREDEINV                 Erro("Lista de MRedes inválida.");
#define MSGERRSYS_LISOBJINV                   Erro("Lista inválida.");
#define MSGERRSYS_LISREDUINV                  Erro("Lista de reduções inválida.");
#define MSGERRSYS_LISSUPORTE                  Erro("Lista de suportes inválida.");
#define MSGERRSYS_MANOBRAINV                  Erro("Manobra inválida.");
#define MSGERRSYS_MANOBRASINV                 Erro("Manobras inválido.");
#define MSGERRSYS_MATCSTREAM                  Erro("Erro na montagem da matriz de capacitâncias.");
#define MSGERRSYS_MATYINV                     Erro("Matriz de admitâncias inválida.");
#define MSGERRSYS_MATZSTREAM                  Erro("Erro na montagem da matriz de impedâncias.");
#define MSGERRSYS_MEDIDORINV                  Erro("Medidor inválido.");
#define MSGERRSYS_MERCADOINV                  Erro("Mercado inválido.");
#define MSGERRSYS_MONTAINV                    Erro("Montador inválido.");
#define MSGERRSYS_MREDEINV                    Erro("MRede inválida.");
#define MSGERRSYS_MUTUABARINV                 Erro("Barra(s) inválido(s) para a mútua " + mutua->Codigo);
#define MSGERRSYS_MUTUAINV                    Erro("Mútua inválida.");
#define MSGERRSYS_MUTUATREINV                 Erro("Trecho(s) inválido(s) para a mútua " + mutua->Codigo);
#define MSGERRSYS_OBRAINV                     Erro("Obra inválida.");
#define MSGERRSYS_OBRAS                       Erro("Obras inválidas.");
#define MSGERRSYS_PATHINV                     Erro("Diretórios inválidos.");
#define MSGERRSYS_PATAMARESINV                Erro("Patamares inválidos.");
#define MSGERRSYS_PERDAINV                    Erro("Perda inválida.");
#define MSGERRSYS_PONTINV                     Erro("Ponteiro inválido.");
#define MSGERRSYS_PTOFIX                      Erro("Ponto de fixação inválido para o suporte " + suporte->Codigo + ".");
#define MSGERRSYS_RADIALINV                   Erro("Radial inválida.");
#define MSGERRSYS_REATORINV                   Erro("Reator inválido.");
#define MSGERRSYS_REDEINV                     Erro("Rede inválida.");
#define MSGERRSYS_REDESINV                    Erro("Redes inválidas.");
#define MSGERRSYS_REDUCAOINV                  Erro("Redução inválida.");
#define MSGERRSYS_SEGMENTOINV                 Erro("Segmento inválida.");
#define MSGERRSYS_REGULADORINV                Erro("Regulador inválido.");
#define MSGERRSYS_REGIAOINV                   Erro("Região inválida.");
#define MSGERRSYS_SESSION                     Erro("Sessão inválida.");
#define MSGERRSYS_SESSIONCON                  Erro("Sessão válida, porém não há conexão com a base de dados.");
#define MSGERRSYS_SINCRONISMOINV              Erro("Sincronismo inválido.");
#define MSGERRSYS_SINCRONISMOSINV             Erro("Sincronismos inválido.");
#define MSGERRSYS_STREAM                      Erro("Erro na montagem dos pontos de uma curva.");
#define MSGERRSYS_SUPORTEINV                  Erro("Suporte inválido.");
#define MSGERRSYS_SUPRIMENTOINV               Erro("Suprimento inválido.");
#define MSGERRSYS_TIPOREDEINV                 Erro("Tipo de rede inválido.");
#define MSGERRSYS_TIPOSINV                    Erro("Tipos inválidos.");
#define MSGERRSYS_TRAFO3EINV                  Erro("Trafo 3E inválido.");
#define MSGERRSYS_TRAFOINV                    Erro("Trafo inválido.");
#define MSGERRSYS_TRAFOMATY                   Erro("Erro na montagem da matriz de admitâncias para o trafo " + trafo->Codigo);
#define MSGERRSYS_TRAFOZZINV                  Erro("TrafoZZ inválido.");
#define MSGERRSYS_TRATAFIMDADOS               Erro("Erro no tratamento final dos dados lidos.");
#define MSGERRSYS_TRECHOINV                   Erro("Trecho inválido.");
#define MSGERRSYS_YREFINV                     Erro("Matriz de admitâncias inválida.");

#define MSGERRBDDL_CORTES                     Erro("Erro na remoção dos cortes.");
#define MSGERRBDDL_DOMREDE                    Erro("Erro na remoção dos domínios da rede " + rede->Codigo);
#define MSGERRBDDL_ESTUDOALT                  Erro("Erro na remoção das alternativas do estudo " + estudo->Codigo);
#define MSGERRBDDL_REGIAO                     Erro("Erro na remoção das regiões");
#define MSGERRBDDL_SEGMENTO                   Erro("Erro na remoção do segmento " + segmento->Codigo);
#define MSGERRBDDL_TRAFOENROL                 Erro("Erro na remoção dos enrolamentos de uma trafo");
#define MSGERRBDRD_ARRANJO                    Erro("Erro na leitura do arranjo");
#define MSGERRBDRD_CABO                       Erro("Erro na leitura do cabo " + str_cabo.codigo);
#define MSGERRBDRD_CLASSE                     Erro("Erro na leitura de uma classe.");
#define MSGERRBDRD_CURTIPICAS                 Erro("Erro na leitura das curvas típicas.");
#define MSGERRBDRD_CURCRESCIMENTO             Erro("Erro na leitura da curva de crescimento.");
#define MSGERRBDRD_DOMINIO                    Erro("Erro na leitura dos domínios.");
#define MSGERRBDRD_ESTUDO                     Erro("Erro na leitura do estudo " + estudo->Codigo);
#define MSGERRBDRD_ESTUDOALTERNATIVA          Erro("Erro na leitura das alternativas do estudo " + estudo->Codigo);
#define MSGERRBDRD_GERAL                      Erro("Erro na leitura das configurações gerais");
#define MSGERRBDRD_MATCARRANJO                Erro("Erro na leitura da matriz C do arranjo " + str_arranjo2.codigo);
#define MSGERRBDRD_MATZARRANJO                Erro("Erro na leitura da matriz Z do arranjo " + str_arranjo2.codigo);
#define MSGERRBDRD_MERCADO                    Erro("Erro na leitura do mercado.");
#define MSGERRBDRD_NUMPONTOSFIX               Erro("Erro no número de pontos de fixação");
#define MSGERRBDRD_POLIGONO                   Erro("Erro na leitura de um polígono.");
#define MSGERRBDRD_REDE                       Erro("Erro na leitura das redes do estudo " + estudo->Codigo);
#define MSGERRBDRD_REDEMREDE                  Erro("Erro na leitura das redes que compõem a MRede " + mrede->Codigo);
#define MSGERRBDRD_RRBARRA                    Erro("Erro na leitura das barras reduzidas.");
#define MSGERRBDRD_RRCARGA                    Erro("Erro na leitura das cargas reduzidas.");
#define MSGERRBDRD_RRESTUDO                   Erro("Erro na leitura das reduções do estudo " + estudo->Codigo);
#define MSGERRBDRD_RRSUPRIMENTO               Erro("Erro na leitura dos suprimentos reduzidos.");
#define MSGERRBDRD_RRTRAFO                    Erro("Erro na leitura dos transformadores reduzidos.");
#define MSGERRBDRD_RRYREF                     Erro("Erro na leitura das matrizes de admitância reduzidas.");
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
#define MSGERRBDWR_DLEQUIPREDE(straux, streq) straux.sprintf("Erro na remoção. Tipo de equipamento: %s, rede: %s, estudo: %d.", streq, rede->Codigo.c_str(), estudo->Id);
#define MSGERRBDWR_DOMINIOS                   Erro("Erro na escrita dos domínios.");
#define MSGERRBDWR_DOMREDE                    Erro("Erro na associação de um domínio à rede " + rede->Codigo);
#define MSGERRBDWR_EQPTO                      Erro("Erro na escrita do equipamento");
#define MSGERRBDWR_ESTUDO                     Erro("Erro na escrita do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOACAO                 Erro("Erro na escrita das ações do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOALT                  Erro("Erro na escrita das alternativas do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOEQUIP                Erro("Erro na escrita dos equipamentos do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOEQUIP                Erro("Erro na escrita dos equipamentos do estudo " + estudo->Codigo);
#define MSGERRBDWR_ESTUDOOBRA                 Erro("Erro na escrita das obras do estudo " + estudo->Codigo);
#define MSGERRBDWR_LIXEIRA                    Erro("Erro na remoção de itens eliminados no estudo " + estudo->Codigo);
#define MSGERRBDWR_POLIGONO                   Erro("Erro na escrita de um polígono");
#define MSGERRBDWR_REDES                      Erro("Erro na escrita das redes do estudo " + estudo->Codigo);
#define MSGERRBDWR_SEGMENTO                   Erro("Erro na gravação da região " + segmento->Codigo);
#define MSGERRBDWR_SEGMENTO_CRESCIMENTO       Erro("Erro na gravação do crescimento " + crescimento->Codigo + " da região " + segmento->Codigo);
#define MSGERRBDWR_RRCARGA                    Erro("Erro na escrita das cargas reduzidas.");
#define MSGERRBDWR_RRSUPRIMENTO               Erro("Erro na escrita dos suprimentos reduzidos.");
#define MSGERRBDWR_RRTRAFO                    Erro("Erro na escrita dos transformadores reduzidos.");
#define MSGERRBDWR_RRYREF                     Erro("Erro na escrita das matrizes de admitância reduzidas.");
#define MSGERRBDWR_SUPORTE                    Erro("Erro na escrita do suporte " + suporte->Codigo);
#define MSGERRBDWR_SUPORTEUTILIZADO           Erro("Erro na escrita do suporte " + suporte->Codigo + ".\nEsse suporte está sendo utilizado.");
#define MSGERRBDWR_WREQUIPREDE(straux, streq) straux.sprintf("Erro na escrita. Tipo de equipamento: %s, estudo: %d.", streq, estudo->Id);

#define MSGERRBD_PARAMINV                     Erro("Parâmetro inválido.");
#define MSGERRBD_EXECINV                      Erro("Erro na execução.");

#endif
