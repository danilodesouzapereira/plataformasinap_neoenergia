  INSERT INTO
    Agrupamento (intEstudoId, intAgrupamentoId, strCodigo, bolAlteradoObra)
  SELECT [p_EstudoIdNovo], intAgrupamentoId, strCodigo, bolAlteradoObra
  FROM Agrupamento WHERE intEstudoId = [p_EstudoIdOrg];
