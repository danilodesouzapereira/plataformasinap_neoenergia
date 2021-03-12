  SELECT
    Cabo.intCaboId                                   AS CaboId,
    Cabo.intTipoCaboId                               AS TipoCaboId,
    Cabo.strDescricao                                AS Descricao,
    Cabo.strExtId                                    AS ExtId,
    Cabo.fltInom                                     AS Inom,
    Cabo.fltVmin                                     AS Vmin,
    Cabo.fltVmax                                     AS Vmax,
    Cabo.intStatus                                   AS Status,
    IIF(ISNULL(CaboRGMR.fltR),   0, CaboRGMR.fltR)   AS RGMR_R,
    IIF(ISNULL(CaboRGMR.fltGMR), 0, CaboRGMR.fltGMR) AS RGMR_GMR,
    0                                                AS Z0Z1_Z0R,
    0                                                AS Z0Z1_Z0X,
    0                                                AS Z0Z1_Z1R,
    0                                                AS Z0Z1_Z1X,
    0                                                AS Z0Z1_C0,
    0                                                AS Z0Z1_C1
  FROM                                    
    Cabo INNER JOIN CaboRGMR ON
      Cabo.intCaboId = CaboRGMR.intCaboId
  WHERE
    (( [p_CaboId] = -1) OR (Cabo.intCaboId = [p_CaboId] ))

  UNION

  SELECT
    Cabo.intCaboId                                   AS CaboId,
    Cabo.intTipoCaboId                               AS TipoCaboId,
    Cabo.strDescricao                                AS Descricao,
    Cabo.strExtId                                    AS ExtId,
    Cabo.fltInom                                     AS Inom,
    Cabo.fltVmin                                     AS Vmin,
    Cabo.fltVmax                                     AS Vmax,
    Cabo.intStatus                                   AS Status,
    0                                                AS RGMR_R,
    0                                                AS RGMR_GMR,
    IIF(ISNULL(CaboZ0Z1.fltZ0R), 0, CaboZ0Z1.fltZ0R) AS Z0Z1_Z0R,
    IIF(ISNULL(CaboZ0Z1.fltZ0X), 0, CaboZ0Z1.fltZ0X) AS Z0Z1_Z0X,
    IIF(ISNULL(CaboZ0Z1.fltZ1R), 0, CaboZ0Z1.fltZ1R) AS Z0Z1_Z1R,
    IIF(ISNULL(CaboZ0Z1.fltZ1X), 0, CaboZ0Z1.fltZ1X) AS Z0Z1_Z1X,
    IIF(ISNULL(CaboZ0Z1.fltC0),  0, CaboZ0Z1.fltC0)  AS Z0Z1_C0,
    IIF(ISNULL(CaboZ0Z1.fltC1),  0, CaboZ0Z1.fltC1)  AS Z0Z1_C1
  FROM
    Cabo INNER JOIN CaboZ0Z1 ON
      Cabo.intCaboId = CaboZ0Z1.intCaboId
  WHERE
    (( [p_CaboId] = -1) OR (Cabo.intCaboId = [p_CaboId] ));
