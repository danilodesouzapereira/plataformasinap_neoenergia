SELECT 
  intRedeId        AS RedeId,
  intCargaId       AS CargaId,
  strCodigo        AS Codigo,
  intClasseId      AS ClasseId,
  intUtmX          AS UtmX,
  intUtmY          AS UtmY,
  intNumConsumidor AS NumConsumidor,
  blobPontos       AS Pontos
FROM 
  RedeCarga 
WHERE
  RedeCarga.dttData = [p_Data];
