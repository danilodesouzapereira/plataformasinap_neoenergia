  SELECT
    GeralPadraoRede.intPadraoRedeId  AS PadraoRedeId,
    GeralPadraoRede.strDescricao     AS Descricao,
    GeralPadraoRede.intNumFases      AS NumFases,
    GeralPadraoRede.fltVFF           AS VFF,
    GeralPadraoRede.fltVFN           AS VFN,
    GeralPadraoRede.fltV1            AS V1,
    GeralPadraoRede.fltV2            AS V2,
    GeralPadraoRede.fltV3            AS V3,
    GeralPadraoRede.fltV4            AS V4,
    GeralPadraoRede.fltFPI1          AS FPI1,
    GeralPadraoRede.fltFPI2          AS FPI2,
    GeralPadraoRede.fltFPS1          AS FPS1,
    GeralPadraoRede.fltFPS2          AS FPS2
  FROM
    GeralPadraoRede;
