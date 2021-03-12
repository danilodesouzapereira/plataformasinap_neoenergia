SELECT
  Rede.strCodigo                     AS RedeCodigo,
  Rede.intTipoRedeId                 AS TipoRedeId,
  TipoRede.strCodigo                 AS TipoRedeCodigo,
  ArvoreNo.intNoId                   AS NoId,
  ArvoreNo.strCodigo                 AS NoCodigo,
  ArvoreNo.intNoPaiId                AS NoPaiId,
  ArvoreNo.intNivelId                AS NoNivelId,
  RedeCarga.intRedeId                AS RedeId,
  RedeCarga.intCargaId               AS CargaId,
  RedeCarga.strCodigo                AS Codigo,
  RedeCarga.intClasseId              AS ClasseId,
  RedeCarga.intUtmX                  AS UtmX,
  RedeCarga.intUtmY                  AS UtmY,
  RedeCarga.intNumConsumidor         AS NumConsumidor,
  RedeCarga.blobPontos               AS Pontos
FROM
  (
  (
  (
  ArvoreNo INNER JOIN RedeCarga ON
     ArvoreNo.intNoId = RedeCarga.intNoId
  )
  INNER JOIN Rede ON
     RedeCarga.intRedeId = Rede.intRedeId
  )
  INNER JOIN TipoRede ON
     Rede.intTipoRedeId = TipoRede.intTipoRedeId
  )
WHERE
  ArvoreNo.intArvoreId = [p_ArvoreId] AND
  (([p_NivelId] = -1) OR (ArvoreNo.intNivelId <= [p_NivelId]))
ORDER BY
  ArvoreNo.intNoId,
  RedeCarga.intRedeId
