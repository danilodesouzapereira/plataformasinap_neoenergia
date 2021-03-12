INSERT INTO
  CurvaSegmentoParamUsuario (intConjCurvaId, intCurvaId, intSegmentoId, intParamId, strDescricao, strValor, strValorMin, strValorMax, bolVarIndep, intTipoDadoId)
SELECT 
  [p_ConjCurvaIdProx], intCurvaId, intSegmentoId, intParamId, strDescricao, strValor, strValorMin, strValorMax, bolVarIndep, intTipoDadoId
FROM
  CurvaSegmentoParamUsuario
WHERE
  intConjCurvaId = [p_ConjCurvaId]