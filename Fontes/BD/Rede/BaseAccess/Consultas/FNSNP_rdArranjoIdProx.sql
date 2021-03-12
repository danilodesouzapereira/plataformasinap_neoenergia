SELECT
  IIF(ISNULL(MAX(intArranjoId)), 0, MAX(intArranjoId)) + 1 AS IdProx 
FROM 
  Arranjo
 

