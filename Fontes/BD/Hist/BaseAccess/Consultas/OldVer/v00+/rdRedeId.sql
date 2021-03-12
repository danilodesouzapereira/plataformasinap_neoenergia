SELECT
    IIF(ISNULL(Rede.intRedeId), -1, Rede.intRedeId) AS RedeId
FROM 
   Rede
WHERE 
   Rede.strCodigo = [p_Codigo];
