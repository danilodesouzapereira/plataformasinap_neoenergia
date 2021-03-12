  INSERT INTO
    Medidor (intEstudoId, intMedidorId)
  SELECT [p_EstudoIdNovo], intMedidorId
  FROM Medidor WHERE intEstudoId = [p_EstudoIdOrg];

