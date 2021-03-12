  INSERT INTO
    RedeDominio (intEstudoId, intRedeId, intDominioId)
  SELECT [p_EstudoIdNovo], intRedeId, intDominioId
  FROM RedeDominio WHERE intEstudoId = [p_EstudoIdOrg];
