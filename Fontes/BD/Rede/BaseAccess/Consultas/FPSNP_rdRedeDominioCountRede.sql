  SELECT
    COUNT(*)
  INTO
    conta
  FROM
    (
    SELECT DISTINCT
      intEstudoId
    FROM
      RedeDominio
    WHERE
      intDominioId = [p_DominioId]
    );
