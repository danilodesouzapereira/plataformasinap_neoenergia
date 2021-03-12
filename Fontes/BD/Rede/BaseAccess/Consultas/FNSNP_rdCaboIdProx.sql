SELECT
  IIF(ISNULL(MAX(intCaboId)), 0, MAX(intCaboId)) + 1 AS IdProx 
FROM 
  Cabo
 

