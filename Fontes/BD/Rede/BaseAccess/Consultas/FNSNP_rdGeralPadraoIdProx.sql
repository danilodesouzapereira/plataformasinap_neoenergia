SELECT
  IIF(ISNULL(MAX(intPadraoRedeId)), 0, MAX(intPadraoRedeId)) + 1 AS IdProx 
FROM 
  GeralPadraoRede
