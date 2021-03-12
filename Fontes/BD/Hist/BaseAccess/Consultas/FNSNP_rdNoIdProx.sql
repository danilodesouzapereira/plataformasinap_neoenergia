SELECT 
  IIF(ISNULL(MAX(intNoId)), 1, MAX(intNoId) + 1) AS IdProx
FROM 
  ArvoreNo
