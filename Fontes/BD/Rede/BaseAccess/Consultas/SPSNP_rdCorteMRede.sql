  SELECT
    Corte.intCorteId AS CorteId
  FROM
    Corte
  WHERE
    Corte.intEstudoId = [p_EstudoId] AND
    Corte.intMRedeId = [p_MRedeId];
