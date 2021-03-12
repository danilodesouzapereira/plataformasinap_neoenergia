SELECT
  IIF(ISNULL(MAX(intCorteId)), 0, MAX(intCorteId)) + 1 AS IdProx 
FROM 
  Corte
WHERE 
  intEstudoId = [p_EstudoId]
 