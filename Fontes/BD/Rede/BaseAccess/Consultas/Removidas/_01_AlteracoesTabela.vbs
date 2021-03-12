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
    strDBFilePath = "..\"
    strDBFileName = strDBFilePath & "..\Rede_v416.mdb"

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
    straux = "ALTER TABLE Canal ADD intEquipId INTEGER NOT NULL"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE Canal ADD intTipoFaseId TINYINT NOT NULL"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE Canal ADD CONSTRAINT FKCanal261187 FOREIGN KEY (intEstudoId, intEquipId) REFERENCES Equipamento (intEstudoId, intEquipId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "ALTER TABLE Canal ADD CONSTRAINT FKCanal532683 FOREIGN KEY (intTipoFaseId) REFERENCES TipoFase (intTipoFaseId)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "INSERT INTO TipoCurva(intTipoCurvaId, intTipoCurvaFamiliaId, strDescricao, intLegendaX, intLegendaY) VALUES (9, 50, 'CurvaV', 12, null)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "INSERT INTO TipoCurva(intTipoCurvaId, intTipoCurvaFamiliaId, strDescricao, intLegendaX, intLegendaY) VALUES (10, 50, 'CurvaI', 12, null)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "INSERT INTO TipoCurva(intTipoCurvaId, intTipoCurvaFamiliaId, strDescricao, intLegendaX, intLegendaY) VALUES (11, 50, 'CurvaP', 12, null)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "INSERT INTO TipoCurva(intTipoCurvaId, intTipoCurvaFamiliaId, strDescricao, intLegendaX, intLegendaY) VALUES (12, 50, 'CurvaQ', 12, null)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    straux = "INSERT INTO TipoCurva(intTipoCurvaId, intTipoCurvaFamiliaId, strDescricao, intLegendaX, intLegendaY) VALUES (13, 50, 'CurvaS', 12, null)"
    cmdSQL.CommandText = straux
    cmdSQL.Execute()
    
    '--
    '-- Etapa 02: Recria as procedures
    '--

    ' --- Iniciação dos paths  ---
    strDirSpec = "Z:\Sinap\Rev4.0_XE\PlataformaSinap\Fontes\BD\Rede\BaseAccess\Consultas"

    ' --- Deleta  as Procedures Pertinentes
    Set catObj.ActiveConnection = DBConn
    catObj.Procedures.Delete "SPSNP_rdCanal_p01"
    catObj.Procedures.Delete "SPSNP_rdCanal_p02"
    catObj.Procedures.Delete "SPSNP_wrCanal_p02"

    ' Para todos arquivos "SQL" existentes no diretório, cria as consultas
    Set fleDir = fleObj.GetFolder(strDirSpec)

    ' Limpa variável de erro
    Err.Clear
    strFileName = strDirSpec + "\SPSNP_rdCanal_p01.SQL"
    'MsgBox strFileName 
    strExtName = UCase(fleObj.GetExtensionName(strFileName))
    'MsgBox strExtName
    strQryName = fleObj.GetBaseName(strFileName)
    Set fleSQL = fleObj.OpenTextFile(strFileName, 1) 'Somente leitura = 1
    strSQL = fleSQL.ReadAll
    Set cmdSQL.ActiveConnection = DBConn
    cmdSQL.CommandText = strSQL
    Set catObj.ActiveConnection = DBConn
    catObj.Procedures.Append strQryName, cmdSQL
    If (Err.Number <> 0) THen
      strErr = "Arquivo " & strFileName & " => Erro " & Err.Number & ": " & Err.Description
      MsgBox strErr
    End If
     
    ' Limpa variável de erro
    Err.Clear
    strFileName = strDirSpec + "\SPSNP_rdCanal_p02.SQL"
    'MsgBox strFileName 
    strExtName = UCase(fleObj.GetExtensionName(strFileName))
    'MsgBox strExtName
    strQryName = fleObj.GetBaseName(strFileName)
    Set fleSQL = fleObj.OpenTextFile(strFileName, 1) 'Somente leitura = 1
    strSQL = fleSQL.ReadAll
    Set cmdSQL.ActiveConnection = DBConn
    cmdSQL.CommandText = strSQL
    Set catObj.ActiveConnection = DBConn
    catObj.Procedures.Append strQryName, cmdSQL
    If (Err.Number <> 0) THen
      strErr = "Arquivo " & strFileName & " => Erro " & Err.Number & ": " & Err.Description
      MsgBox strErr
    End If
     
    ' Limpa variável de erro
    Err.Clear
    strFileName = strDirSpec + "\SPSNP_wrCanal_p02.SQL"
    'MsgBox strFileName 
    strExtName = UCase(fleObj.GetExtensionName(strFileName))
    'MsgBox strExtName
    strQryName = fleObj.GetBaseName(strFileName)
    Set fleSQL = fleObj.OpenTextFile(strFileName, 1) 'Somente leitura = 1
    strSQL = fleSQL.ReadAll
    Set cmdSQL.ActiveConnection = DBConn
    cmdSQL.CommandText = strSQL
    Set catObj.ActiveConnection = DBConn
    catObj.Procedures.Append strQryName, cmdSQL
    If (Err.Number <> 0) THen
      strErr = "Arquivo " & strFileName & " => Erro " & Err.Number & ": " & Err.Description
      MsgBox strErr
    End If
     
     'Clean up
     DBconn.Close
     Set DBconn = Nothing
     Set cmdSQL = Nothing
     Set catObj = Nothing
     Set fleObj = Nothing

    MsgBox "Final da execução."
