  DELETE FROM
    Equipamento
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1));
