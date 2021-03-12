SELECT
  IIF(ISNULL(MAX(intManobraId)), 0, MAX(intManobraId)) + 1 AS IdProx 
FROM 
  Manobra
 