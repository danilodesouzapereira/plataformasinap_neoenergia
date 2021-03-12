SELECT
  IIF(ISNULL(MAX(intFixacaoItem)), 0, MAX(intFixacaoItem)) + 1 AS IdProx 
FROM 
  SuporteFixacao
WHERE 
  intSuporteId = [p_SuporteId];
 