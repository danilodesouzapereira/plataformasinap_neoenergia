On Error Resume Next

    Dim DBconn         ' Conex�o ao banco
    Dim strDBFileName  ' Nome do arquivo que armazena o banco
    Dim strDBFilePath  ' Path do arquivo que armazena o banco
    Dim fleObj         ' Objeto para manipula��o de arquivos
    Dim fleSQL         ' Arquivo de entrada (Cont�m o script SQL)
    Dim strDirSpec     ' Especifica��o do diret�rio para busca dos arquivos
    Dim fleDir         ' Diret�rio para busca dos arquivos
    Dim fleList        ' Lista de arquivos existentes no diret�rio
    Dim fleIter        ' Arquivo existente no diret�rio
    Dim strQryName     ' Nome da consulta (que corresponde, praticamente, ao nome do arquivo)
    Dim strFileName    ' Nome do arquivo, formatado
    Dim strExtName     ' Nome da extens�o arquivo
    Dim strSQL         ' Comando SQl lido do arquivo
    Dim strPWd         ' String que cont�m a senha de acesso ao banco
    Dim catQry         ' Cat�logo de stored procedures (queries COM par�metros) ou Views (queries sem par�metros) - ADO
    Dim cmdSQL         ' Conte�do do objeto (view ou procedure) - ADO
    Dim qry            ' Stored Procedure ou View
    Dim intCount       ' N�mero de elementos na cole��o

    ' Setup dos objetos
    ' ADO cria consultas invis�veis
    Set fleObj = CreateObject("Scripting.FileSystemObject")
    Set catQry = CreateObject("ADOX.Catalog")
    Set cmdSQL = CreateObject("ADODB.Command")

    ' --- Inicia��o dos paths  ---
    'strDirSpec    = ""
    strDirSpec = "."
    
    strDBFilePath = "..\"
    strDBFileName = strDBFilePath & "Historico.mdb"

    ' --- Digita��o dos dados pelo usu�rio ---
    'strDirSpec    = InputBox("Digite o caminho do diret�rio dos arquivos SQL", "Banco de dados", strDirSpec) 
    strDBFileName = InputBox("Digite o caminho completo do arquivo MDB", "Banco de dados", strDBFileName) 
    'strPWd        = InputBox("Digite a senha (aten��o: o conte�do digitado ser� visualizado)", "Senha de acesso ao banco", "") 
    strPWd        = "ultraplan2006"

    ' --- Conex�o com o banco  ---
    ' Open the Database Connection: ReadOnly = False
    Set DBConn = CreateObject("ADODB.Connection")
    DBConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & strDBFileName & "; Jet OLEDB:Database Password=" & strPWd

   
    ' --- Loop das queries  ---

    ' --- Deleta todas as Procedures e Views existentes
    Set catQry.ActiveConnection = DBConn
    For intCount = 0 To catQry.Procedures.Count - 1
      catQry.Procedures.Delete 0
    Next

    Set catQry.ActiveConnection = DBConn
    For intCount = 0 To catQry.Views.Count - 1
      catQry.Views.Delete 0
    Next

    ' Para todos arquivos "SQL" existentes no diret�rio, cria as consultas
    Set fleDir = fleObj.GetFolder(strDirSpec)
    Set fleList = fleDir.Files
    For Each fleIter in fleList
      ' Limpa vari�vel de erro
      Err.Clear

      strFileName = fleIter.Name 
      'MsgBox strFileName 
      strExtName = UCase(fleObj.GetExtensionName(strFileName))
      'MsgBox strExtName
      If (strExtName = "SQL") Then
        strQryName = fleObj.GetBaseName(strFileName)
        Set fleSQL = fleObj.OpenTextFile(strFileName, 1) 'Somente leitura = 1
        strSQL = fleSQL.ReadAll
          
        Set cmdSQL.ActiveConnection = DBConn
        cmdSQL.CommandText = strSQL

        Set catQry.ActiveConnection = DBConn
        If (Instr(strSQL, "@") > 0) Then
          catQry.Procedures.Append strQryName, cmdSQL
        Else
          catQry.Views.Append strQryName, cmdSQL
        End If
        
        If (Err.Number <> 0) THen
          strErr = "Arquivo " & strFileName & " => Erro " & Err.Number & ": " & Err.Description
          MsgBox strErr
          Exit For
        End If
 
      End If 
    Next

   
    'Clean up
    DBconn.Close
    Set DBconn = Nothing
    Set fleObj = Nothing
    Set catQry = Nothing
    Set cmdSQL = Nothing

    MsgBox "Final da execu��o."
