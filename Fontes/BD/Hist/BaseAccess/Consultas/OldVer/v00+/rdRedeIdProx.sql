SELECT 
  IIF(ISNULL(MAX(intRedeId)), 1, MAX(intRedeId) + 1) AS intRedeIdProx
FROM 
  Rede
