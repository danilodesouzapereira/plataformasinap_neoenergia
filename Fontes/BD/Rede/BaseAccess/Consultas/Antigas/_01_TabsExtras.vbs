'    On Error Resume Next 

    Dim DBconn         ' Conexão ao banco
    Dim cmdSQL         ' Comando SQL A ser executado
    Dim catTab         ' Catálogo de tabelas

    Dim strDirSpec     ' Especificação do diretório para busca dos arquivos
    Dim strDBFilePath  ' Path do arquivo que armazena o banco
    Dim strDBFileName  ' Nome do arquivo que armazena o banco
    Dim strPWd         ' String que contém a senha de acesso ao banco

    Dim straux         ' String auxiliar


    ' Setup dos objetos
    Set catTab = CreateObject("ADOX.Catalog")
    Set cmdSQL = CreateObject("ADODB.Command")

    ' --- Iniciação dos paths  ---
    strDBFilePath = "..\"
    strDBFileName = strDBFilePath & "Rede_v412a.mdb"

    ' --- Digitação dos dados pelo usuário ---
    strDBFileName = InputBox("Digite o caminho completo do arquivo MDB", "Banco de dados", strDBFileName) 
    strPWd        = InputBox("Digite a senha (atenção: o conteúdo digitado será visualizado)", "Senha de acesso ao banco", "sinap2006_2013") 

    ' --- Conexão com o banco  ---
    ' Open the Database Connection: ReadOnly = False
    Set DBConn = CreateObject("ADODB.Connection")
    DBConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & strDBFileName & "; Jet OLEDB:Database Password=" & strPWd

    Set cmdSQL.ActiveConnection = DBConn
    Set catTab.ActiveConnection = DBConn

    '--
    '-- Etapa 01: Cria tabela
    '--

    ' Não pode fazer o loop de tabelas, pois tem ordem certa para apagar as tabelas.
    ' Integridade referencial

    For intCount = 0 To catTab.Tables.Count - 1
      straux = catTab.Tables(intCount).Name

      If (straux = "TmpEquip") Then
        cmdSQL.CommandText = "DROP TABLE TmpEquip"
        cmdSQL.Execute()
      End If
      
      If (straux = "TmpCurva") Then
        cmdSQL.CommandText = "DROP TABLE TmpCurva"
        cmdSQL.Execute()
      End If
      
      If (straux = "TmpEquipCorte") Then
        cmdSQL.CommandText = "DROP TABLE TmpEquipCorte"
        cmdSQL.Execute()
      End If
      
      If (straux = "TmpEstudoRedeCorte") Then
        cmdSQL.CommandText = "DROP TABLE TmpEstudoRedeCorte"
        cmdSQL.Execute()
      End If
      
    Next

    straux = "CREATE TABLE TmpEquip (intEquipId INTEGER NOT NULL)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE TmpEquip ADD PRIMARY KEY (intEquipId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()

    straux = "CREATE TABLE TmpCurva (intConjCurvaId INTEGER NOT NULL)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE TmpCurva ADD PRIMARY KEY (intConjCurvaId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()

    straux = "CREATE TABLE TmpEquipCorte (intMRedeId INTEGER NOT NULL, intEquipId INTEGER NOT NULL, intCorteId INTEGER NOT NULL)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE TmpEquipCorte ADD PRIMARY KEY (intMRedeId, intEquipId, intCorteId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()

    straux = "CREATE TABLE TmpEstudoRedeCorte (intEstudoId INTEGER NOT NULL, intMRedeId INTEGER NOT NULL, intCorteId INTEGER NOT NULL)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE TmpEstudoRedeCorte ADD PRIMARY KEY (intEstudoId, intMRedeId, intCorteId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()

    'Clean up
    DBconn.Close
    Set DBconn = Nothing
    Set cmdSQL = Nothing

    MsgBox "Concluído."
