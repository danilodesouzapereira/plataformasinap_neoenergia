On Error Resume Next

    Dim DBconn         ' Conexão ao banco
    Dim strDBFileName  ' Nome do arquivo que armazena o banco
    Dim strDBFilePath  ' Path do arquivo que armazena o banco
    Dim fleObj         ' Objeto para manipulação de arquivos
    Dim fleSQL         ' Arquivo de entrada (Contém o script SQL)
    Dim strDirSpec     ' Especificação do diretório para busca dos arquivos
    Dim fleDir         ' Diretório para busca dos arquivos
    Dim fleList        ' Lista de arquivos existentes no diretório
    Dim fleIter        ' Arquivo existente no diretório
    Dim strQryName     ' Nome da consulta (que corresponde, praticamente, ao nome do arquivo)
    Dim strFileName    ' Nome do arquivo, formatado
    Dim strExtName     ' Nome da extensão arquivo
    Dim strSQL         ' Comando SQl lido do arquivo
    Dim strPWd         ' String que contém a senha de acesso ao banco
    Dim catQry         ' Catálogo de stored procedures (queries COM parâmetros) ou Views (queries sem parâmetros) - ADO
    Dim cmdSQL         ' Conteúdo do objeto (view ou procedure) - ADO
    Dim qry            ' Stored Procedure ou View
    Dim intCount       ' Número de elementos na coleção

    ' Setup dos objetos
    ' ADO cria consultas invisíveis
    Set fleObj = CreateObject("Scripting.FileSystemObject")
    Set catQry = CreateObject("ADOX.Catalog")
    Set cmdSQL = CreateObject("ADODB.Command")

    ' --- Iniciação dos paths  ---
    'strDirSpec    = ""
    strDirSpec = "."
    
    strDBFilePath = "..\"
    strDBFileName = strDBFilePath & "Historico.mdb"

    ' --- Digitação dos dados pelo usuário ---
    'strDirSpec    = InputBox("Digite o caminho do diretório dos arquivos SQL", "Banco de dados", strDirSpec) 
    strDBFileName = InputBox("Digite o caminho completo do arquivo MDB", "Banco de dados", strDBFileName) 
    'strPWd        = InputBox("Digite a senha (atenção: o conteúdo digitado será visualizado)", "Senha de acesso ao banco", "") 
    strPWd        = "ultraplan2006"

    ' --- Conexão com o banco  ---
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

    ' Para todos arquivos "SQL" existentes no diretório, cria as consultas
    Set fleDir = fleObj.GetFolder(strDirSpec)
    Set fleList = fleDir.Files
    For Each fleIter in fleList
      ' Limpa variável de erro
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

    MsgBox "Final da execução."
