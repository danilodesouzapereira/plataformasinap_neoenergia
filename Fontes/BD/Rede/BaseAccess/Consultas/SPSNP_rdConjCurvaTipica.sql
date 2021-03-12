  SELECT
    ConjuntoCurva.intConjCurvaId         AS ConjCurvaId,
    ConjuntoCurva.strDescricao           AS Descricao,
    ConjuntoCurvaSegmento.intTipoCurvaId AS TipoCurvaId,
    ConjuntoCurva.intStatus              AS Status,
    ConjuntoCurvaSegmento.blbPontos      AS Pontos
  FROM
    ConjuntoCurva INNER JOIN ConjuntoCurvaSegmento ON
      ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
  WHERE
    ConjuntoCurvaSegmento.intCurvaId = 1 AND                 
    ConjuntoCurvaSegmento.intSegmentoId = 1 AND              
    ConjuntoCurvaSegmento.intTipoCurvaId BETWEEN 1 AND 7 AND 
    ConjuntoCurva.bolTipica = 1;
