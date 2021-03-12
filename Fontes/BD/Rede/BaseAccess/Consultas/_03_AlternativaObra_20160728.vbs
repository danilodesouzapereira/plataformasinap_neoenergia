'    On Error Resume Next 

    Dim DBconn         ' Conexão ao banco
    Dim cmdSQL         ' Comando SQL A ser executado
    Dim catObj         ' Catálogo de tabelas

    Dim strDirSpec     ' Especificação do diretório para busca dos arquivos
    Dim strDBFilePath  ' Path do arquivo que armazena o banco
    Dim strDBFileName  ' Nome do arquivo que armazena o banco
    Dim strPWd         ' String que contém a senha de acesso ao banco
    Dim strFileName    ' Nome do arquivo, formatado
    Dim strExtName     ' Nome da extensão arquivo

    Dim straux         ' String auxiliar


    ' Setup dos objetos
    Set catObj = CreateObject("ADOX.Catalog")
    Set cmdSQL = CreateObject("ADODB.Command")
    Set fleObj = CreateObject("Scripting.FileSystemObject")

    ' --- Iniciação dos paths  ---
    'strDBFilePath = "Z:\Sinap\Rev4.0_XE\PlataformaSinap\Fontes\BD\Rede\BaseAccess\"
	strDBFilePath = "Z:\SINAPgrid\PlataformaSinap\Fontes\BD\Rede\BaseAccess\"
    strDBFileName = strDBFilePath & "Rede_v417-.mdb"

    ' --- Digitação dos dados pelo usuário ---
    strDBFileName = InputBox("Digite o caminho completo do arquivo MDB", "Banco de dados", strDBFileName) 
    strPWd        = InputBox("Digite a senha (atenção: o conteúdo digitado será visualizado)", "Senha de acesso ao banco", "sinap2006_2013") 

    ' --- Conexão com o banco  ---
    ' Open the Database Connection: ReadOnly = False
    Set DBConn = CreateObject("ADODB.Connection")
    DBConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & strDBFileName & "; Jet OLEDB:Database Password=" & strPWd

    Set cmdSQL.ActiveConnection = DBConn
    Set catObj.ActiveConnection = DBConn

    '--
    '-- Etapa 01: Altera tabela 
    '--
    straux = "ALTER TABLE AlternativaObra ADD bolAlteraTaxaFalha TINYINT NOT NULL"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE AlternativaObra ADD fltTaxaFalhaIns FLOAT NOT NULL"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE AlternativaObra ADD fltTaxaFalhaSub FLOAT NOT NULL"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    'Clean up
    DBconn.Close
    Set DBconn = Nothing
    Set cmdSQL = Nothing
    Set catObj = Nothing
    Set fleObj = Nothing

    MsgBox "Final da execução."
