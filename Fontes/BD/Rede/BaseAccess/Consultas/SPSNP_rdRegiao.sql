  SELECT
    Regiao.intRegiaoId                   AS RegiaoId,
    Regiao.intTipoRegiaoId               AS TipoRegiaoId,
    Regiao.strCodigo                     AS Codigo,
    Regiao.intUtm1X                      AS Utm1X,
    Regiao.intUtm1Y                      AS Utm1Y,
    Regiao.intUtm2X                      AS Utm2X,
    Regiao.intUtm2Y                      AS Utm2Y,
    TipoConsumidor.intTag                AS TipoConsTag,
    RegiaoCrescimento.intConjCurvaId     AS ConjCurvaId,
    ConjuntoCurvaSegmento.blbPontos      AS Pontos,
    Regiao.intAreaGeoId                  AS AreaGeoId
  FROM
    (
    (
    (
    Regiao
      INNER JOIN RegiaoCrescimento ON
        Regiao.intRegiaoId = RegiaoCrescimento.intRegiaoId
    )

      INNER JOIN ConjuntoCurvaSegmento ON
        RegiaoCrescimento.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
    )

      INNER JOIN TipoConsumidor ON
        RegiaoCrescimento.intTipoConsId = TipoConsumidor.intTipoConsId
    )
  ORDER BY
    Regiao.intRegiaoId;
