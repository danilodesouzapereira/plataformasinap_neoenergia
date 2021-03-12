SELECT
  IIF(ISNULL(MAX(intRegiaoId)), 0, MAX(intRegiaoId)) + 1 AS IdProx 
FROM 
  Regiao
