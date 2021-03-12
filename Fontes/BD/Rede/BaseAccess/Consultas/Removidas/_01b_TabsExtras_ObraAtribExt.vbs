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
    strDBFileName = strDBFilePath & "Rede_v417.mdb"

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

      If (straux = "ObraAtribExt") Then
        cmdSQL.CommandText = "DROP TABLE ObraAtribExt"
        cmdSQL.Execute()
      End If
      
    Next

    straux = "CREATE TABLE ObraAtribExt (intEstudoId TINYINT NOT NULL, intAlternativaId tinyint NOT NULL, intObraId tinyint NOT NULL, intExtId int NOT NULL, intAtribId int NOT NULL, blbExt image NULL )"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE ObraAtribExt ADD PRIMARY KEY (intEstudoId, intAlternativaId, intObraId, intExtId, intAtribId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()

    'straux = "ALTER TABLE ObraAtribExt ADD CONSTRAINT FKObraAtribExt FOREIGN KEY (intEstudoId, intAlternativaId, intObraId) REFERENCES AlternativaObra (intEstudoId, intAlternativaId, intObraId)"
    'cmdSQL.CommandText = straux
    'cmdSQL.Execute()
    
    'Clean up
    DBconn.Close
    Set DBconn = Nothing
    Set cmdSQL = Nothing

    MsgBox "Concluído."
