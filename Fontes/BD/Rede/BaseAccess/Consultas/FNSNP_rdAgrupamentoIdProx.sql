SELECT 
  IIF(ISNULL(MAX(intAgrupamentoId)), 0, MAX(intAgrupamentoId)) + 1 AS IdProx
FROM 
  Agrupamento

