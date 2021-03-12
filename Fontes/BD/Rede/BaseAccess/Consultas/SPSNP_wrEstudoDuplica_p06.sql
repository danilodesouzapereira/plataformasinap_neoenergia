  INSERT INTO
    BarraRede (intEstudoId, intRedeId, intBarraId)
  SELECT [p_EstudoIdNovo], intRedeId, intBarraId
  FROM BarraRede WHERE intEstudoId = [p_EstudoIdOrg];
