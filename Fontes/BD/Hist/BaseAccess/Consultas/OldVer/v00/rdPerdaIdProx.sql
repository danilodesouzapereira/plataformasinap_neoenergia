SELECT 
  IIF(ISNULL(MAX(Rede_id)), 1, MAX(Rede_id) + 1) AS intPerdaIdProx
FROM 
  Perda
