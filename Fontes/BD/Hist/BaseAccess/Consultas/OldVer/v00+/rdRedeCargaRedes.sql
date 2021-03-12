SELECT DISTINCT 
  Rede.intRedeId     AS RedeId,
  Rede.intTipoRedeId AS TipoRedeId,
  Rede.strCodigo     AS Codigo
FROM 
  RedeCarga INNER JOIN Rede ON
    RedeCarga.intRedeId = Rede.intRedeId
WHERE
  RedeCarga.dttData = [p_Data];
