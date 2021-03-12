  SELECT
    Corte.intMRedeId AS MRedeId
  FROM
    Corte
  WHERE
    Corte.intEstudoId = [p_EstudoId] AND
    Corte.intCorteId = [p_CorteId];
