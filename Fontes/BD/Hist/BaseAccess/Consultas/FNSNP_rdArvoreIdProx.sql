SELECT 
  IIF(ISNULL(MAX(intArvoreId)), 1, MAX(intArvoreId) + 1) AS IdProx
FROM 
  Arvore
