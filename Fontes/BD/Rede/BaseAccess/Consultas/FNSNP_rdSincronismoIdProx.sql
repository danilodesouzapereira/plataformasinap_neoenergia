SELECT
  IIF(ISNULL(MAX(intSincronismoId)), 0, MAX(intSincronismoId)) + 1 AS IdProx 
FROM 
  Sincronismo
 