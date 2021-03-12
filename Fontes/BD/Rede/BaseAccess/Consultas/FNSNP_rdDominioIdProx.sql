SELECT
  IIF(ISNULL(MAX(intDominioId)), 0, MAX(intDominioId)) + 1 AS IdProx 
FROM 
  Dominio
 

