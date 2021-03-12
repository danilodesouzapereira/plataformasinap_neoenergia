SELECT
  IIF(ISNULL(MAX(intEstudoId)), 0, MAX(intEstudoId)) + 1 AS IdProx
FROM 
  Estudo
 

