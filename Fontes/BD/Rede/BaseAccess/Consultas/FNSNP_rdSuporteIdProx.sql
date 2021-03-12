SELECT
  IIF(ISNULL(MAX(intSuporteId)), 0, MAX(intSuporteId)) + 1 AS IdProx 
FROM 
  Suporte
 

