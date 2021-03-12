  DELETE FROM
    Arranjo_02_04
  WHERE
    intArranjoId IN
    (
    SELECT
      Arranjo.intArranjoId
    FROM
      (
      (
      Trecho
        INNER JOIN Equipamento ON
          Trecho.intEstudoId = Equipamento.intEstudoId AND
          Trecho.intTrechoId = Equipamento.intEquipId
      )

        INNER JOIN Arranjo ON
          Trecho.intArranjoId = Arranjo.intArranjoId
      )
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Arranjo.bolTipico = 0
    );